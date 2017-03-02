/**
 * \file test_compare_int8.cpp
 *
 * Unit tests for compare_int8.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two int8_t values that are equal results in 0.
 */
TEST(compare_int8, equality)
{
    const int8_t X = 17;
    const int8_t Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(int8_t)));
    EXPECT_EQ(0, compare_int8(&X, &Y, sizeof(int8_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_int8, greater_than)
{
    const int8_t X = 17;
    const int8_t Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(int8_t)));
    EXPECT_LT(0, compare_int8(&X, &Y, sizeof(int8_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_int8, less_than)
{
    const int8_t X = 17;
    const int8_t Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(int8_t)));
    EXPECT_GT(0, compare_int8(&X, &Y, sizeof(int8_t)));
}
