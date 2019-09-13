/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include <Arduino.h>
#include "embedded_tools/n64/adrian_single_wire_arduino.hpp"
#include "embedded_tools/n64/adrian_n64_controller.hpp"
#include "embedded_tools/adrian_helpers.hpp"

using adrian::zero_bit;
using adrian::one_bit;
using adrian::stop_bit;
using adrian::N64Controller;


/* ===== Setup ===== */

void setup()
{
    Serial.begin(115200);
    // Serial.setTimeout(10); // Set timeout to 10 milliseconds.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(3, OUTPUT);
}

static unsigned char reverse_lookup[16] = {
0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };

uint8_t reverse(uint8_t n) {
   // Reverse the top and bottom nibble then swap them.
   return (reverse_lookup[n&0b1111] << 4) | reverse_lookup[n>>4];
}


/* ===== Loop =====  */

int loop_counter = 0;
void loop()
{
    pinMode(2, OUTPUT);

    uint32_t button_status = 0;
    uint32_t mask = 1;
    volatile uint8_t i = 0;

    noInterrupts();

    zero_bit(); // 0
    zero_bit(); // 1
    zero_bit(); // 2
    zero_bit(); // 3
    zero_bit(); // 4
    zero_bit(); // 5
    zero_bit(); // 6
    one_bit(); // 7
    PORTD |= 0x08;
    stop_bit();
    PORTD &= 0xF7;

    // Configure the pins for input
    // 0 for input
    // 1 for output
    //   DDRC = DDRC & Mask;

    //   // Enable the pullups
    //   PORTC = PORTC | Mask;


    // pinMode(2, INPUT_PULLUP);

    i = 1; i = 2; i = 3; i = 4;
    i = 1; i = 2; i = 3; i = 4;
    i = 1; i = 2; i = 3; i = 4;
    i = 1; i = 2; i = 3; i = 4;
    for (uint8_t timeout = 0; timeout < 100 && (PIND & 4); timeout++);
    i = 1; i = 2; i = 3; i = 4;
    i = 1; i = 2; i = 3; i = 4;

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

    interrupts();

    // Copy button info
    // Example:
    N64Controller::ButtonState buttons;
    memcpy(&buttons, &button_status, sizeof(buttons));
    buttons.joy_x = reverse(buttons.joy_x);
    buttons.joy_y = reverse(buttons.joy_y);

    Serial.print(loop_counter++);
    Serial.print(": ");
    if (buttons.a)       Serial.print("A, ");
    if (buttons.b)       Serial.print("B, ");
    if (buttons.z)       Serial.print("Z, ");
    if (buttons.start)   Serial.print("S, ");
    if (buttons.d_up)    Serial.print("DU, ");
    if (buttons.d_down)  Serial.print("DD, ");
    if (buttons.d_left)  Serial.print("DL, ");
    if (buttons.d_right) Serial.print("DR, ");
    if (buttons.l)       Serial.print("L, ");
    if (buttons.r)       Serial.print("R, ");
    if (buttons.c_up)    Serial.print("CU, ");
    if (buttons.c_down)  Serial.print("CD, ");
    if (buttons.c_left)  Serial.print("CL, ");
    if (buttons.c_right) Serial.print("CR, ");
    Serial.println();

    Serial.print("X: ");
    Serial.println(buttons.joy_x, DEC);
    Serial.print("Y: ");
    Serial.println(buttons.joy_y, DEC);

    // Serial.println(button_status, HEX);

    // Wait
    delay(100);
}
