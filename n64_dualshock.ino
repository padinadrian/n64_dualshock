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

void loop()
{
    pinMode(2, OUTPUT);

    uint32_t button_status = 0;

    zero_bit(); // 0
    zero_bit(); // 1
    zero_bit(); // 2
    zero_bit(); // 3
    zero_bit(); // 4
    zero_bit(); // 5
    zero_bit(); // 6
    zero_bit(); // 7
    stop_bit();

    pinMode(2, INPUT_PULLUP);

    for (uint32_t mask = 1; mask > 0; mask <<= 1)
    {
        button_status |= ((PIND & 4) ? mask : 0);
    }

    Serial.println(button_status);

    // Wait
    delay(100);
}
