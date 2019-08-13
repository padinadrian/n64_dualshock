/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include <Arduino.h>
#include "embedded_tools/n64/adrian_single_wire_arduino.hpp"
#include "embedded_tools/adrian_helpers.hpp"

using adrian::zero_bit;
using adrian::one_bit;
using adrian::stop_bit;


/* ===== Setup ===== */

void setup()
{
    Serial.begin(115200);
    // Serial.setTimeout(10); // Set timeout to 10 milliseconds.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(3, OUTPUT);
}


/* ===== Loop =====  */

int loop_counter = 0;
void loop()
{
    pinMode(2, OUTPUT);

    uint32_t button_status = 0;
    uint32_t mask = 1;
    volatile uint8_t i = 0;

    zero_bit(); // 0
    zero_bit(); // 1
    zero_bit(); // 2
    zero_bit(); // 3
    zero_bit(); // 4
    zero_bit(); // 5
    zero_bit(); // 6
    one_bit(); // 7
    stop_bit();

    // Configure the pins for input
    //   DDRC = DDRC & Mask;

    //   // Enable the pullups
    //   PORTC = PORTC | Mask;

    DDRD &= 0xFB;       // Configure as input
    PORTD |= 0x04;      // Set to high (enable pull-up)

    // pinMode(2, INPUT_PULLUP);

    // i = 1; i = 2; i = 3; i = 4;
    // i = 1; i = 2; i = 3; i = 4;
    // i = 1; i = 2; i = 3; i = 4;
    // i = 1; i = 2; i = 3; i = 4;
    for (; mask > 0; mask <<= 1)
    {
        PORTD |= 0x08;
        button_status |= ((PIND & 4) ? mask : 0);
        PORTD &= 0xF7;
        i = 1; i = 2; i = 3; i = 4; i = 5;
        i = 1; i = 2; i = 3; i = 4; i = 5;
        i = 1; i = 2; i = 3; i = 4; i = 5;
        i = 1; i = 2; i = 3; i = 4; i = 5;
        i = 1;
    }

    // Copy button info
    // Example:
    ButtonState buttons;
    memcpy(&buttons, &button_status, sizeof(buttons));

    Serial.print(loop_counter++);
    Serial.print(": ");


    // Serial.println(button_status, HEX);

    // Wait
    delay(100);
}
