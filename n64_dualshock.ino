/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include <Arduino.h>
#include "embedded_tools/n64/adrian_single_wire_arduino.hpp"

/*
void Transfer(uint8_t* tx_buffer, uint8_t num_bytes)
{
    for (uint8_t idx = 0; idx < num_bytes; idx++)
    {
        uint8_t byte = tx_buffer[idx];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            // Pull low for 1us to indicate new bit.
            pin.digitalWrite(LOW);
            MicroSleep(1);
            // Pull low or high depending on bit state.
            pin.digitalWrite((byte && (1 << bit)) > 0);
            MicroSleep(2);
            // Pull high for 1us to indicate end of bit.
            pin.digitalWrite(HIGH);
            MicroSleep(1);
        }
    }
}
*/

/*
    N64 Console Commands

    0x00 --> Identify
    0x01 --> Data
    0xFF --> Recalibrate joystick
    0x02 --> Memory read
    0x03 --> Memory write

*/

/*
    N64 Controller Button Data:

    bit 0  --> A
    bit 1  --> B
    bit 2  --> Z
    bit 3  --> S
    bit 4  --> D UP
    bit 5  --> D DOWN
    bit 6  --> D LEFT
    bit 7  --> D RIGHT

    bit 8  --> unused
    bit 9  --> unused
    bit 10 --> L
    bit 11 --> R
    bit 12 --> C UP
    bit 13 --> C DOWN
    bit 14 --> C LEFT
    bit 15 --> C RIGHT

    byte 3 --> JOY X (range -0x50 to 0x50)
    byte 4 --> JOY Y (range -0x50 to 0x50)
*/

/*
    PS2 Joystick range = -128 --> 127 or 0xFF --> 0x7F
    N64 Joystick range = -80 --> 80 or -0x50 (0xB0) --> 0x50

*/
/*
void TranslateButtons(
    const DualShock::ButtonState ps2_buttons,
    N64::ButtonState n64_buttons)
{
    // Joystick
    n64_buttons.joy_x = ps2_buttons.analog_left_x;
    n64_buttons.joy_y = ps2_buttons.analog_left_y;

    // Buttons
    n64_buttons.a = ps2_buttons.cross;
    n64_buttons.b = ps2_buttons.square;
    n64_buttons.z = ps2_buttons.left1;
    n64_buttons.r = ps2_buttons.right1;
    n64_buttons.l = ps2_buttons.left2;
    n64_buttons.start = ps2_buttons.start;
    n64_buttons.d_down = ps2_buttons.select;

    n64_buttons.c_up = ps2_buttons.d_up;
    n64_buttons.c_down = ps2_buttons.d_down;
    n64_buttons.c_left = ps2_buttons.d_left;
    n64_buttons.c_right = ps2_buttons.d_right;
}
*/

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(1000000);
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.write("hello");
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.write("goodbyte");
    delay(1000);
}
