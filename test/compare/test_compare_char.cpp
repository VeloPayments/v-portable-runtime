/**
 * \file test_compare_char.cpp
 *
 * Unit tests for compare_char.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0
/**
 * Test that comparing two characters that are equal results in 0.
 */
TEST(compare_char, equality)
{
    const char X = 17;
    const char Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(char)));
    EXPECT_EQ(0, compare_char(&X, &Y, sizeof(char)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_char, greater_than)
{
    const char X = 17;
    const char Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(char)));
    EXPECT_LT(0, compare_char(&X, &Y, sizeof(char)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_char, less_than)
{
    const char X = 17;
    const char Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(char)));
    EXPECT_GT(0, compare_char(&X, &Y, sizeof(char)));
}
#endif
