/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#define EMBEDDED_TOOLS_BASE_DIRECTORY "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\"
// #include EMBEDDED_TOOLS_BASE_DIRECTORY "/gpio/adrian_spi_trinket.hpp"
// #include EMBEDDED_TOOLS_BASE_DIRECTORY "/spi/adrian_spi_trinket.hpp"
// #include "ps2/adrian_dualshock.hpp"
// #include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/n64/adrian_n64_console.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/gpio/adrian_gpio_arduino.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/spi/adrian_spi_trinket.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/ps2/adrian_dualshock.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/n64/adrian_n64_console.hpp"

/* ===== Global ===== */

static adrian::TrinketSPI spi;
static adrian::ArduinoGPIO spi_select(3);
static adrian::DualShock controller(&spi, &spi_select);

/* ===== Helpers ===== */

#define N64_PIN 4
#define PIN_MASK (1 << N64_PIN)
#define N64_HIGH  (DDRB &= ~PIN_MASK)  // set to input
#define N64_LOW   (DDRB |= PIN_MASK)   // set to output
#define N64_CHECK (PINB & PIN_MASK)    // read pin

// Write up to 255 bytes
void SingleWireWrite(uint8_t *buf, uint8_t bufsize)
{
    uint8_t out;
    uint8_t mask;
    uint8_t i;
    for (i = 0; i < bufsize; ++i) {
        for (mask = 0x80; mask > 0;) {
            N64_LOW;
            if (buf[i] & mask) {
                asm volatile ("nop\n");
                N64_HIGH;
                asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\n");
                asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\n");
                asm volatile ("nop\nnop\n");
            } else {
                asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\n");
                asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\n");
                asm volatile ("nop\nnop\nnop\n");
                N64_HIGH;
            }
            // To delay, or not to delay? Always, except the last bit.
            mask >>= 1;
            if (mask) {
                asm volatile ("nop\nnop\n");
            }
        }
    }

    // Send a single stop (1) bit
    N64_LOW;
    // wait 1 us, 16 cycles, then raise the line
    // 16-2=14
    // nop block 6
    asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\n");
    asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\n");
    N64_HIGH;
}

// Read a command
// Returns the number of bytes read
uint8_t SingleWireRead(uint8_t *buf, const uint8_t bufsize)
{
    // If timeout is nonzero, console pulled down
    static uint8_t input_buf[8];
    uint8_t timeout;

    while (N64_CHECK);

    for (uint8_t i = 0; i < 8; ++i) {

        // Wait for line to drop low
        timeout = 255;
        while (N64_CHECK && (timeout--));
        if (timeout == 0) { break; }

        // TODO: Wait 2 ms
        asm volatile ("nop\nnop\nnop\nnop\n");

        // Read a bit
        PINB |= 0b00001000;
        input_buf[i] = PINB;
        PINB &= 0b11110111;

        // Wait to go back up and then down
        PINB |= 0b00001000;
        while (!N64_CHECK);
        PINB &= 0b11110111;
    }

    // Reconstruct message
    for (uint8_t i = 0; i < 8; ++i) {
        if (input_buf[i] & PIN_MASK) {
            *buf |= (1 << i);
        }
    }

    return 1;
}


/* ===== SETUP ===== */

void setup()
{
    digitalWrite(N64_PIN, 0);
    pinMode(N64_PIN, INPUT);
    spi.Initialize();

    pinMode(0, INPUT);
    pinMode(1, OUTPUT);
    spi_select.SetPinMode(OUTPUT);
    spi_select.Write(0);

    noInterrupts();
}


/* ===== LOOP ===== */

#define OUTPUT_BUFSIZE 4

static uint8_t input_buf = 0;
static uint8_t output_buf[OUTPUT_BUFSIZE] = { 0 };

static adrian::DualShock::ButtonState ps2_buttons;
static adrian::N64Controller::ButtonState n64_buttons;

static uint8_t status_buf[3] = { 0x05, 0x00, 0x00 };
static uint8_t read_response[33] = { 0 };

void loop()
{
    int32_t tmp;

    // Poll from PS2 controller
    if (!controller.IsAnalogEnabled()) {
        controller.EnableAnalog();
    }
    else {
        controller.Poll(ps2_buttons);
    }

    if (ps2_buttons.analog_valid) {
        n64_buttons.a = ps2_buttons.cross;
        n64_buttons.b = ps2_buttons.square;
        n64_buttons.z = ps2_buttons.left2;
        n64_buttons.start = ps2_buttons.start;
        n64_buttons.c_up = ps2_buttons.d_up;
        n64_buttons.c_down = ps2_buttons.d_down;
        n64_buttons.c_left = ps2_buttons.d_left;
        n64_buttons.c_right = ps2_buttons.d_right;
        n64_buttons.l = ps2_buttons.left1;
        n64_buttons.r = ps2_buttons.right2;

        tmp = ps2_buttons.analog_left_x;
        tmp = ((tmp - 0x80) * 0x50 / 0x80);
        n64_buttons.joy_x = tmp;

        tmp = ps2_buttons.analog_left_y;
        tmp = ((tmp - 0x80) * 0x50 / 0x80);
        n64_buttons.joy_y = tmp;
    }
    else {
        // Set all buttons and joysticks to zero
        memset((void *)(&n64_buttons), 0, sizeof(n64_buttons));
    }

    // Respond to commands from N64
    noInterrupts();

    SingleWireRead(&input_buf, 1);
    switch (input_buf) {
        // Status
        case 0xFF:      // Fall-through
        case 0x00: {
            SingleWireWrite(status_buf, sizeof(status_buf));
            break;
        }
        // Poll
        case 0x01: {
            // output_buf[1] = input_buf;
            SingleWireWrite((uint8_t*)(&n64_buttons), sizeof(n64_buttons));
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
            // Nothing
            break;
        }
    }

    interrupts();
}
