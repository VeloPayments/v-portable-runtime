/**
 * \file test_compare_unsigned_int.cpp
 *
 * Unit tests for compare_unsigned_int.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two unsigned ints that are equal results in 0.
 */
TEST(compare_unsigned_int, equality)
{
    const unsigned int X = 17;
    const unsigned int Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(unsigned int)));
    EXPECT_EQ(0, compare_unsigned_int(&X, &Y, sizeof(unsigned int)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_unsigned_int, greater_than)
{
    const unsigned int X = 17;
    const unsigned int Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(unsigned int)));
    EXPECT_LT(0, compare_unsigned_int(&X, &Y, sizeof(unsigned int)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_unsigned_int, less_than)
{
    const unsigned int X = 17;
    const unsigned int Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(unsigned int)));
    EXPECT_GT(0, compare_unsigned_int(&X, &Y, sizeof(unsigned int)));
}
