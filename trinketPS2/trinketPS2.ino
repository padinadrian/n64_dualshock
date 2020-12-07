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

    controller.EnableAnalog();

    noInterrupts();
}


/* ===== LOOP ===== */

#define OUTPUT_BUFSIZE 4

static uint8_t input_buf = 0;
static uint8_t output_buf[OUTPUT_BUFSIZE] = {0};


void loop()
{
    // Poll from PS2 controller
    static adrian::DualShock::ButtonState buttons;
    controller.Poll(buttons);

    if (buttons.digital_valid) {
        input_buf = buttons.cross;
    }
    else {
        input_buf = 0;
    }

    // delay(10);

    noInterrupts();

    SingleWireRead(&input_buf, 1);
    output_buf[1] = input_buf;
    SingleWireWrite(output_buf, 4);

    interrupts();
}
