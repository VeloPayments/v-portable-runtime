/**
 * \file test_compare_int16.cpp
 *
 * Unit tests for compare_int16.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two int16_t values that are equal results in 0.
 */
TEST(compare_int16, equality)
{
    const int16_t X = 17;
    const int16_t Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(int16_t)));
    EXPECT_EQ(0, compare_int16(&X, &Y, sizeof(int16_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_int16, greater_than)
{
    const int16_t X = 17;
    const int16_t Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(int16_t)));
    EXPECT_LT(0, compare_int16(&X, &Y, sizeof(int16_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_int16, less_than)
{
    const int16_t X = 17;
    const int16_t Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(int16_t)));
    EXPECT_GT(0, compare_int16(&X, &Y, sizeof(int16_t)));
}
