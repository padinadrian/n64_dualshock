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
#include "embedded_tools/ps2/adrian_dualshock.hpp"

using adrian::zero_bit;
using adrian::one_bit;
using adrian::stop_bit;
using adrian::N64Controller;
using adrian::DualShock;


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

    // Wait
    delay(100);
}
