#include <gtest/gtest.h>
#include "button_translate.hpp"
using namespace adrian;


/* ===== Tests ===== */

TEST(ButtonTranslateTests, TranslateJoystick1)
{
    EXPECT_EQ(TranslateJoystick(0), -128);
}

TEST(ButtonTranslateTests, TranslateJoystick2)
{
    EXPECT_EQ(TranslateJoystick(128), 0);
}

TEST(ButtonTranslateTests, TranslateJoystick3)
{
    EXPECT_EQ(TranslateJoystick(255), 127);
}