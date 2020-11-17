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
static adrian::ArduinoGPIO led(4);
static adrian::DualShock controller(&spi, &spi_select);

/* ===== SETUP ===== */

void setup()
{
    led.SetPinMode(adrian::GPIO::PIN_MODE_OUTPUT);
    led.Write(1);
    spi.Initialize();

    pinMode(0, INPUT);
    pinMode(1, OUTPUT);
    noInterrupts();
}

/* ===== LOOP ===== */

static uint8_t byte1 = 0;
static uint8_t button_data[4] = {0};
volatile uint8_t i = 0;

template <uint8_t mask, uint8_t pin>
inline void ReadBit()
{
    const static uint8_t pin_mask = 1 << pin;
    // const static not_pin_mask =
    // Spin while pin 1 is high
    // PORTB |= 0b00000010;
    while (PINB & pin_mask);

    i = 1; i = 2;

    // Read bit from pin 0
    // PORTB &= 0b11111101;
    byte1 |= (PINB & pin_mask) ? mask : 0x0;
    // PORTB |= 0b00000010;

    // Spin while pin 1 is low
    while (!(PINB & pin_mask));
}

template <uint32_t mask, uint8_t pin, uint8_t byte_number>
inline void WriteBit()
{
    const static uint8_t pin_mask = 1 << pin;

    PORTB &= ~pin_mask;
    PORTB |= ((button_data[byte_number] & mask) ? pin_mask : 0);
    // if (button_data[byte_number] & mask)
    // {
    //     PORTB |= 0x1;
    // }
    // else
    // {
    //     PORTB |= 0x0;
    // }
    // Timing stuff...
    i = 1; i = 2; i = 3; i = 4;
    i = 1; i = 2; i = 3; i = 4;
    i = 1; i = 2;
    PORTB |= pin_mask;
}

void loop()
{
    // Poll from PS2 controller
    // static adrian::DualShock::ButtonState buttons;
    // controller.Poll(buttons);

    // if (buttons.digital_valid && buttons.cross)
    // {
    //     led.Write(1);
    // }
    // else
    // {
    //     led.Write(0);
    // }

    // Wait for command
    // pinMode(4, INPUT);
    // pinMode(4, INPUT_PULLUP);
    DDRB &= ~(1 << 4);
    PORTB |=

    // byte1 = 0;
    // ReadBit<0x01, 4>();
    // ReadBit<0x02, 4>();
    // ReadBit<0x04, 4>();
    // ReadBit<0x08, 4>();
    // ReadBit<0x10, 4>();
    // ReadBit<0x20, 4>();
    // ReadBit<0x40, 4>();
    // ReadBit<0x80, 4>();

    // button_data[0] = 0x05;
    // button_data[1] = 0x00;
    // button_data[2] = 0x01;  // 0x80 when flipped
    // button_data[3] = 0x00;
    // DDRB |= (1 << 4);

    // // timing stuff
    // // for (volatile int i = 0; i < 10; i++);

    // // byte 0
    // WriteBit<0x01, 4, 0>();
    // WriteBit<0x02, 4, 0>();
    // WriteBit<0x04, 4, 0>();
    // WriteBit<0x08, 4, 0>();
    // WriteBit<0x10, 4, 0>();
    // WriteBit<0x20, 4, 0>();
    // WriteBit<0x40, 4, 0>();
    // WriteBit<0x80, 4, 0>();

    // // byte 1
    // WriteBit<0x01, 4, 1>();
    // WriteBit<0x02, 4, 1>();
    // WriteBit<0x04, 4, 1>();
    // WriteBit<0x08, 4, 1>();
    // WriteBit<0x10, 4, 1>();
    // WriteBit<0x20, 4, 1>();
    // WriteBit<0x40, 4, 1>();
    // WriteBit<0x80, 4, 1>();

    // // byte 2
    // WriteBit<0x01, 4, 2>();
    // WriteBit<0x02, 4, 2>();
    // WriteBit<0x04, 4, 2>();
    // WriteBit<0x08, 4, 2>();
    // WriteBit<0x10, 4, 2>();
    // WriteBit<0x20, 4, 2>();
    // WriteBit<0x40, 4, 2>();
    // WriteBit<0x80, 4, 2>();

    // // byte 3
    // WriteBit<0x01, 4, 3>();
    // WriteBit<0x02, 4, 3>();
    // WriteBit<0x04, 4, 3>();
    // WriteBit<0x08, 4, 3>();
    // WriteBit<0x10, 4, 3>();
    // WriteBit<0x20, 4, 3>();
    // WriteBit<0x40, 4, 3>();
    // WriteBit<0x80, 4, 3>();

    // interrupts();
}
