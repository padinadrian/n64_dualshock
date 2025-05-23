/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include <Arduino.h>
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock\\trinketPS2\\N64.hpp"

/* ===== Helpers ===== */

#define N64_PIN   4
#define N64_MASK  (1 << N64_PIN)
#define N64_HIGH  (DDRB &= ~N64_MASK)  // set to input
#define N64_LOW   (DDRB |= N64_MASK)   // set to output
#define N64_CHECK (PINB & N64_MASK)    // read pin

// The definitions below were just for testing, but since
// removing them affects the timing, I've decided to leave
// them in place for now.
#define TEST_PIN0  0
#define PIN0_MASK  (1 << TEST_PIN0)
#define PIN0_HIGH  (DDRB &= ~PIN0_MASK)  // set to input
#define PIN0_LOW   (DDRB |= PIN0_MASK)   // set to output

#define TEST_PIN1  1
#define PIN1_MASK   (1 << TEST_PIN1)
#define PIN1_HIGH   (PORTB |= PIN1_MASK)
#define PIN1_LOW    (PORTB &= ~PIN1_MASK)
#define PIN1_INPUT  (DDRB &= ~PIN1_MASK)  // set to input
#define PIN1_OUTPUT (DDRB |= PIN1_MASK)   // set to output

/* ===== Functions ===== */

// Initialize digital pins
void SingleWireInit()
{
    digitalWrite(N64_PIN, 0);
    pinMode(N64_PIN, INPUT);
}

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
    PIN0_HIGH;
    PINB |= PIN0_MASK;

    return 1;
}
