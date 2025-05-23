/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\trinketPS2\\button_translate.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\trinketPS2\\N64.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/gpio/adrian_gpio_arduino.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/spi/adrian_spi_trinket.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/ps2/adrian_dualshock.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/n64/adrian_n64_console.hpp"

/* ===== Global ===== */

static adrian::TrinketSPI spi;
static adrian::ArduinoGPIO spi_select(3);
static adrian::DualShock controller(&spi, &spi_select);


/* ===== SETUP ===== */

void setup()
{
    spi_select.SetPinMode(OUTPUT);
    spi_select.Write(0);

    SingleWireInit();

    controller.EnableAnalog();
}


/* ===== LOOP ===== */

#define OUTPUT_BUFSIZE 4

static uint8_t input_buf = 0;
static uint8_t output_buf[OUTPUT_BUFSIZE] = { 0 };

static adrian::DualShock::ButtonState ps2_buttons;
static adrian::N64Controller::ButtonState n64_buttons;

static uint8_t status_buf[3] = { 0x05, 0x00, 0x02 };
static uint8_t read_response[33] = { 0 };

void loop()
{
    bool read_controller = false;

    noInterrupts();

    // Respond to commands from N64
    if (SingleWireRead(&input_buf, 1)) {
        switch (input_buf) {
            // Status
            case 0x00: {
                status_buf[1] = 0;
                SingleWireWrite(status_buf, sizeof(status_buf));
                read_controller = true;
                break;
            }
            // Poll
            case 0x01: {
                // output_buf[1] = input_buf;
                SingleWireWrite((uint8_t*)(&n64_buttons), sizeof(n64_buttons));
                read_controller = false;
                break;
            }
            // Read
            case 0x02: {
                SingleWireWrite(read_response, sizeof(read_response));
                break;
            }
            // Write
            case 0x03: {
                // TODO
                break;
            }
            default: {
                break;
                // This is for debugging SingleWireRead.
                // Echo back what you thought the command was.
                status_buf[1] = input_buf;
                SingleWireWrite(status_buf, sizeof(status_buf));
                break;
            }
        }
    }

    interrupts();

    // Poll from PS2 controller
    if (!controller.IsAnalogEnabled()) {
        controller.EnableAnalog();
    }
    else if (read_controller) {
        controller.Poll(ps2_buttons);
        read_controller = false;
    }

    if (ps2_buttons.analog_valid) {
        adrian::TranslateButtons(ps2_buttons, n64_buttons);
    }
    else {
        // Set all buttons and joysticks to zero
        memset((void *)(&n64_buttons), 0, sizeof(n64_buttons));
    }
}
