#include <gtest/gtest.h>
#include "../trinketPS2/button_translate.hpp"
using namespace adrian;


/* ===== Tests ===== */

TEST(ButtonTranslateTests, TranslateJoystick1)
{
    EXPECT_EQ(TranslateJoystick(0), -0x50);
}

TEST(ButtonTranslateTests, TranslateJoystick2)
{
    EXPECT_EQ(TranslateJoystick(0x40), -0x28);
}

TEST(ButtonTranslateTests, TranslateJoystick3)
{
    EXPECT_EQ(TranslateJoystick(0x80), 0);
}

TEST(ButtonTranslateTests, TranslateJoystick4)
{
    EXPECT_EQ(TranslateJoystick(0xC0), 0x28);
}

TEST(ButtonTranslateTests, TranslateJoystick5)
{
    EXPECT_EQ(TranslateJoystick(0xFF), 0x50);
}