/**
 * \file test_compare_unsigned_long.cpp
 *
 * Unit tests for compare_unsigned_long.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two unsigned longs that are equal results in 0.
 */
TEST(compare_unsigned_long, equality)
{
    const unsigned long X = 17;
    const unsigned long Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(unsigned long)));
    EXPECT_EQ(0, compare_unsigned_long(&X, &Y, sizeof(unsigned long)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_unsigned_long, greater_than)
{
    const unsigned long X = 17;
    const unsigned long Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(unsigned long)));
    EXPECT_LT(0, compare_unsigned_long(&X, &Y, sizeof(unsigned long)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_unsigned_long, less_than)
{
    const unsigned long X = 17;
    const unsigned long Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(unsigned long)));
    EXPECT_GT(0, compare_unsigned_long(&X, &Y, sizeof(unsigned long)));
}
