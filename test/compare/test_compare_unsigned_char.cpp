/**
 * \file test_compare_unsigned_char.cpp
 *
 * Unit tests for compare_unsigned_char.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two unsigned characters that are equal results in 0.
 */
TEST(compare_unsigned_char, equality)
{
    const unsigned char X = 17;
    const unsigned char Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(unsigned char)));
    EXPECT_EQ(0, compare_unsigned_char(&X, &Y, sizeof(unsigned char)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_unsigned_char, greater_than)
{
    const unsigned char X = 17;
    const unsigned char Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(unsigned char)));
    EXPECT_LT(0, compare_unsigned_char(&X, &Y, sizeof(unsigned char)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_unsigned_char, less_than)
{
    const unsigned char X = 17;
    const unsigned char Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(unsigned char)));
    EXPECT_GT(0, compare_unsigned_char(&X, &Y, sizeof(unsigned char)));
}
