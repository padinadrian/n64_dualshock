/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock/embedded_tools/ps2/adrian_dualshock.hpp"
#include "C:\\Users\\apadi\\Documents\\projects\\n64_dualshock/embedded_tools/spi/adrian_spi_trinket.hpp"


/* ===== Global ===== */

adrian::TrinketSPI spi;
adrian::DualShock controller(&spi);
adrian::DualShock::ButtonState buttons;

/* ===== SETUP ===== */

void setup()
{
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);

    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);

    spi.Initialize();

//    USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1));
//    USICR |= _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
//
//    SPI_DDR_PORT |= _BV(USCK_DD_PIN);
//    SPI_DDR_PORT |= _BV(DO_DD_PIN);
//    SPI_DDR_PORT &= ~_BV(DI_DD_PIN);
//
//    USICR &= ~_BV(USICS0);
}

/* ===== LOOP ===== */

void loop()
{
//    while (!controller.IsConnected())
//    {
//        if (!controller.Connect())
//        {
//            delay(1000);
//        }
//    }

    digitalWrite(3, LOW);
    controller.Poll(buttons);
    digitalWrite(3, HIGH);

    if (buttons.digital_valid && buttons.cross)
    {
        digitalWrite(4, HIGH);
    }
    else
    {
        digitalWrite(4, LOW);
    }

//    USIDR = 0xA5;
//    USISR = _BV(USIOIF);
//
//    volatile int i = 0;
//
//    noInterrupts();
//    while ( !(USISR & _BV(USIOIF)) ) { USICR |= _BV(USITC); i = 1; i = 1; }
//    interrupts();    
//
//    uint8_t data[] = {0xA5, 0xA5};
//    spi.Transfer(data, data, 2);

    delay(100);
}
