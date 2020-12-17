#include <gtest/gtest.h>
#include "../trinketPS2/button_translate.hpp"
using namespace adrian;


/* ===== Tests ===== */

/* ===== TranslateJoystick ===== */

/* All the way left */
TEST(ButtonTranslateTests, TranslateJoystick1)
{
    EXPECT_EQ(TranslateJoystick(0), -0x50);
}

/* Half-way left */
TEST(ButtonTranslateTests, TranslateJoystick2)
{
    EXPECT_EQ(TranslateJoystick(0x40), -0x28);
}

/* Middle (neutral) */
TEST(ButtonTranslateTests, TranslateJoystick3)
{
    EXPECT_EQ(TranslateJoystick(0x80), 0);
}

/* Half-way right */
TEST(ButtonTranslateTests, TranslateJoystick4)
{
    EXPECT_EQ(TranslateJoystick(0xC0), 0x28);
}

/* All the way right */
TEST(ButtonTranslateTests, TranslateJoystick5)
{
    EXPECT_EQ(TranslateJoystick(0xFF), 0x50);
}


/* ===== TranslateButtons ===== */

TEST(ButtonTranslateTests, TranslateButtons1)
{
    DualShock::ButtonState ps2_buttons = {};
    N64Controller::ButtonState n64_buttons = {};

    // Press some random buttons on the PS2 controller.
    ps2_buttons.cross = 1;
    ps2_buttons.square = 1;
    ps2_buttons.right1 = 1;
    ps2_buttons.left2 = 1;

    // Move the analog stick to a weird position.
    // This should be slightly up and left.
    ps2_buttons.analog_left_x = 0x60;
    ps2_buttons.analog_left_y = 0x20;

    TranslateButtons(ps2_buttons, n64_buttons);

    EXPECT_TRUE(n64_buttons.a);
    EXPECT_TRUE(n64_buttons.b);
    EXPECT_TRUE(n64_buttons.z);
    EXPECT_TRUE(n64_buttons.c_up);

    EXPECT_EQ(n64_buttons.joy_x, -20);
    EXPECT_EQ(n64_buttons.joy_y, 60);
}