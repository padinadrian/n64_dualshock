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


/* ===== Helper Functions ===== */

#define read_bit()\
{\
    PORTD &= 0xF7;\
    button_status |= (((PIND & 4) >> 2) << index);\
    PORTD |= 0x08;\
}

#define zero_bit()\
{\
    PORTD &= 0xFB;\
    i++; i++; i++; i++; i++;\
    PORTD |= 0x04;\
    i++;\
}

#define one_bit()\
{\
    PORTD &= 0xFB;\
    i++;\
    PORTD |= 0x04;\
    i++; i++; i++; i++; i++;\
}

#define stop_bit()\
{\
    PORTD &= 0xFB;\
    i++; i++; i++;\
    PORTD |= 0x04;\
    i++; i++; i++;\
}


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
    volatile int i = 0;

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

    for (uint8_t index = 0; index < 32; index++)
    {
        read_bit();
    }

    Serial.println(button_status);

    // Wait
    delay(100);
}
