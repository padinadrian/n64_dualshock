/**
 * File:    adrian_gpio_arduino.hpp
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

#ifndef N64_DUALSHOCK_BUTTON_TRANSLATE_HPP_
#define N64_DUALSHOCK_BUTTON_TRANSLATE_HPP_

/* ===== Includes ===== */
#include "../embedded_tools/ps2/adrian_dualshock.hpp"
#include "../embedded_tools/n64/adrian_n64_controller.hpp"

using adrian::DualShock;
using adrian::N64Controller;

namespace adrian
{
    /** Translate from PS2 joystick to N64 joystick. */
    inline int8_t TranslateJoystick(uint8_t ps2_joy)
    {
        return static_cast<int8_t>(ps2_joy - 128);
    }

    /** Translate input data from Ps2 controller to N64 format. */
    inline void TranslateButtons(
        const DualShock::ButtonState& ps2_buttons_in,
        N64Controller::ButtonState& n64_buttons_out)
    {
        // Translate digital button data.
        n64_buttons_out.a = ps2_buttons_in.cross;
        n64_buttons_out.b = ps2_buttons_in.circle;
        n64_buttons_out.start = ps2_buttons_in.start;

        // Translate analog joystick data.
        // 0x00 --> 0x80  ---    0 --> -128
        // 0x80 --> 0x00  ---  128 -->    0
        // 0xFF --> 0x7F  ---  255 -->  127
        // n64_buttons_out.joy_x
    }
}

#endif  // N64_DUALSHOCK_BUTTON_TRANSLATE_HPP_
