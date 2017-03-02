/**
 * \file test_compare_uint8.cpp
 *
 * Unit tests for compare_uint8.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two uint8_t values that are equal results in 0.
 */
TEST(compare_uint8, equality)
{
    const uint8_t X = 17;
    const uint8_t Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(uint8_t)));
    EXPECT_EQ(0, compare_uint8(&X, &Y, sizeof(uint8_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_uint8, greater_than)
{
    const uint8_t X = 17;
    const uint8_t Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(uint8_t)));
    EXPECT_LT(0, compare_uint8(&X, &Y, sizeof(uint8_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_uint8, less_than)
{
    const uint8_t X = 17;
    const uint8_t Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(uint8_t)));
    EXPECT_GT(0, compare_uint8(&X, &Y, sizeof(uint8_t)));
}
