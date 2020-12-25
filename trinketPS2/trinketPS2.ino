/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\trinketPS2\\button_translate.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/gpio/adrian_gpio_arduino.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/spi/adrian_spi_trinket.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/ps2/adrian_dualshock.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\embedded_tools/n64/adrian_n64_console.hpp"

/* ===== Global ===== */

static adrian::TrinketSPI spi;
static adrian::ArduinoGPIO spi_select(3);
static adrian::DualShock controller(&spi, &spi_select);

/* ===== Helpers ===== */

#define N64_PIN   4
#define N64_MASK  (1 << N64_PIN)
#define N64_HIGH  (DDRB &= ~N64_MASK)  // set to input
#define N64_LOW   (DDRB |= N64_MASK)   // set to output
#define N64_CHECK (PINB & N64_MASK)    // read pin

// The definitions below were just for testing, but since
// removing them affects the timing, I've decided to leave
// them in place for now.
#define TEST_PIN0  3
#define PIN0_MASK  (1 << TEST_PIN0)
#define PIN0_HIGH  (DDRB &= ~PIN0_MASK)  // set to input
#define PIN0_LOW   (DDRB |= PIN0_MASK)   // set to output

#define TEST_PIN1  1
#define PIN1_MASK   (1 << TEST_PIN1)
#define PIN1_HIGH   (PORTB |= PIN1_MASK)
#define PIN1_LOW    (PORTB &= ~PIN1_MASK)
#define PIN1_INPUT  (DDRB &= ~PIN1_MASK)  // set to input
#define PIN1_OUTPUT (DDRB |= PIN1_MASK)   // set to output


// Write up to 255 bytes
void SingleWireWrite(uint8_t *buf, uint8_t bufsize)
{
    uint8_t flag = 1;
    uint8_t mask;
    uint8_t i;
    for (i = 0; flag; ) {
        for (mask = 0x80; mask > 0;) {
            N64_LOW;
            if (buf[i] & mask) {
                if (mask > 1) {
                    asm volatile ("nop\nnop\n");
                    N64_HIGH;
                    asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\n");
                    asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\n");
                    asm volatile ("nop\nnop\nnop\n");
                }
                else {
                    asm volatile ("nop\n");
                    N64_HIGH;
                    ++i;
                    flag = (i < bufsize);
                    asm volatile ("nop\nnop\nnop\nnop\n");
                    asm volatile ("nop\nnop\nnop\n");
                }
            } else {
                if (mask > 1) {
                    asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\n");
                    asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\n");
                    asm volatile ("nop\nnop\nnop\n");
                    N64_HIGH;
                    asm volatile ("nop\nnop\nnop\n");
                }
                else {
                    ++i;
                    flag = (i < bufsize);
                    asm volatile ("nop\nnop\nnop\nnop\n");
                    asm volatile ("nop\nnop\nnop\n");
                    N64_HIGH;
                }
            }
            mask >>= 1;
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

// Read a command from the N64 console.
// Returns the number of bytes read.
// (For now only supports reading a single byte)
uint8_t SingleWireRead(uint8_t *buf, const uint8_t bufsize)
{
    // If timeout is nonzero, console pulled down
    static uint8_t input_buf[8];

    // Set pin0 and pin1 to output
    // Set pin0 and pin1 to low
    PIN1_OUTPUT;
    PIN1_HIGH;
    PIN1_LOW;
    PIN0_LOW;

    // Wait for line to drop low
    while (N64_CHECK);

    for (uint8_t i = 0; i < 8; ++i) {

        // TODO: Wait 2 ms
        asm volatile ("nop\nnop\nnop\nnop\nnop\n");

        // Read a bit
        PIN0_HIGH;
        input_buf[i] = N64_CHECK;
        PIN0_LOW;

        // This is just for testing.
        // However we need to keep it to get consistent timing.
        if (input_buf[i]) {
            PIN1_HIGH;
        }
        else {
            PIN1_LOW;
        }

        // Wait to go back up and then down
        asm volatile ("nop\nnop\nnop\n");
    }

    // Reconstruct message
    if (input_buf[7]) {
        *buf = 1;
    }
    else {
        *buf = 0;
    }

    // Set pin0 and pin1 to output
    // Set pin0 and pin1 to high
    PIN1_OUTPUT;
    PIN1_HIGH;
    PIN0_LOW;
    PINB |= PIN0_MASK;

    return 1;
}


/* ===== SETUP ===== */

void setup()
{
    spi_select.SetPinMode(OUTPUT);
    spi_select.Write(0);

    digitalWrite(N64_PIN, 0);
    pinMode(N64_PIN, INPUT);

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
    int32_t tmp;

    noInterrupts();

    // Respond to commands from N64
    if (SingleWireRead(&input_buf, 1)) {
        switch (input_buf) {
            // Status
            case 0xFF:      // Fall-through
            case 0x00: {
                status_buf[1] = 0;
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
    else {
        controller.Poll(ps2_buttons);
    }

    if (ps2_buttons.analog_valid) {
        adrian::TranslateButtons(ps2_buttons, n64_buttons);
    }
    else {
        // Set all buttons and joysticks to zero
        memset((void *)(&n64_buttons), 0, sizeof(n64_buttons));
    }
}
