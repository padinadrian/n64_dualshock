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
    /**
     * Translate from PS2 joystick to N64 joystick.
     * The PS2 range is 0x00 to 0xFF with 0x80 being the neutral position.
     * The N64 range is -0x50 to 0x50 with 0x00 being the neutral position.
     * Examples:
     *      0x00 --> -0x50 = -80
     *      0x40 --> -0x28 = -40
     *      0x80 --> 0x00 = 0
     *      0xC0 --> 0x28 = 40
     *      0xFF --> 0x50 = 80
     */
    inline int8_t TranslateJoystick(uint8_t ps2_joy)
    {
        int32_t tmp;
        if (ps2_joy == 0xFF) {
            tmp = 0x50;
        }
        else {
            tmp = static_cast<int32_t>(ps2_joy);
            tmp = ((tmp - 0x80) * 0x50 / 0x80);
        }
        return static_cast<int8_t>(tmp);
    }

    /** Translate input data from Ps2 controller to N64 format. */
    inline void TranslateButtons(
        const DualShock::ButtonState& ps2_buttons_in,
        N64Controller::ButtonState& n64_buttons_out)
    {
        // Translate digital button data.
        n64_buttons_out.a = ps2_buttons_in.cross;
        n64_buttons_out.b = ps2_buttons_in.square;
        n64_buttons_out.z = ps2_buttons_in.left2;
        n64_buttons_out.start = ps2_buttons_in.start;
        n64_buttons_out.c_up = ps2_buttons_in.select;
        n64_buttons_out.c_down = ps2_buttons_in.circle;
        n64_buttons_out.c_left = ps2_buttons_in.triangle;
        n64_buttons_out.c_right = ps2_buttons_in.right1;
        n64_buttons_out.d_up = ps2_buttons_in.d_up;
        n64_buttons_out.d_down = ps2_buttons_in.d_down;
        n64_buttons_out.d_left = ps2_buttons_in.d_left;
        n64_buttons_out.d_right = ps2_buttons_in.d_right;
        n64_buttons_out.l = ps2_buttons_in.left1;
        n64_buttons_out.r = ps2_buttons_in.right2;

        // Translate analog joystick data.
        n64_buttons_out.joy_x = TranslateJoystick(ps2_buttons_in.analog_left_x);
        // For some reason the Y axis has to be inverted.
        n64_buttons_out.joy_y = -TranslateJoystick(ps2_buttons_in.analog_left_y);
    }
}

#endif  // N64_DUALSHOCK_BUTTON_TRANSLATE_HPP_
