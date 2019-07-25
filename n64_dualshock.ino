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

const uint8_t ZERO_ZERO = 0x88; // 0001 0001
const uint8_t ZERO_ONE  = 0xE8; // 0001 0111
const uint8_t ONE_ZERO  = 0x8E; // 0111 0001
const uint8_t ONE_ONE   = 0xEE; // 0111 0111
const uint8_t STOP_BIT  = 0xFC; // 0011 1111

void setup()
{
    Serial.begin(1000000);
    Serial.setTimeout(10); // Set timeout to 10 milliseconds.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(3, OUTPUT);
}

void wait_microsecond()
{
    volatile int counter = 0;
}

const uint8_t tx_buffer[] = {
    ZERO_ZERO, ZERO_ZERO, ZERO_ZERO, ZERO_ZERO,
    STOP_BIT
};

uint32_t button_status;
uint8_t index = 0;

void handler()
{
    rx_buffer[index++] = PIND;
}

void loop()
{
    pinMode(2, OUTPUT);

    index = 0;
    volatile int i = 0;

    // 0
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 1
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 2
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 3
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 4
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 5
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 6
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // 7
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    // Stop bit
    PORTD &= 0xFB;
    i++; i++; i++; i++; i++;
    PORTD |= 0x04;
    i++;

    pinMode(2, INPUT_PULLUP);

    // Collect data through
    delay(1);


    // Serial.write(tx_buffer, sizeof(tx_buffer));
    // pinMode(1, INPUT_PULLUP);
    // Serial.readBytes(rx_buffer, sizeof(rx_buffer));

    // if (rx_buffer[4] == ONE_ONE)
    // {
    //     digitalWrite(LED_BUILTIN, HIGH);
    // }
    // else
    // {
    //     digitalWrite(LED_BUILTIN, LOW);
    // }


    delay(20);

    // digitalWrite(3, LOW);
    // wait_microsecond();
    // wait_microsecond();
    // wait_microsecond();
    // digitalWrite(3, HIGH);
    // wait_microsecond();

    // digitalWrite(3, LOW);
    // wait_microsecond();
    // wait_microsecond();
    // wait_microsecond();
    // digitalWrite(3, HIGH);
    // wait_microsecond();

    // digitalWrite(3, LOW);
    // wait_microsecond();
    // wait_microsecond();
    // wait_microsecond();
    // digitalWrite(3, HIGH);
    // wait_microsecond();

    // digitalWrite(3, LOW);
    // wait_microsecond();
    // wait_microsecond();
    // wait_microsecond();
    // digitalWrite(3, HIGH);
    // wait_microsecond();

    // // Stop bit
    // digitalWrite(3, LOW);
    // wait_microsecond();
    // wait_microsecond();
    // digitalWrite(3, HIGH);
    // wait_microsecond();
    // wait_microsecond();

    // delay(100);
}
