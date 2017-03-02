/**
 * \file test_compare_int.cpp
 *
 * Unit tests for compare_int.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two ints that are equal results in 0.
 */
TEST(compare_int, equality)
{
    const int X = 17;
    const int Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(int)));
    EXPECT_EQ(0, compare_int(&X, &Y, sizeof(int)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_int, greater_than)
{
    const int X = 17;
    const int Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(int)));
    EXPECT_LT(0, compare_int(&X, &Y, sizeof(int)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_int, less_than)
{
    const int X = 17;
    const int Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(int)));
    EXPECT_GT(0, compare_int(&X, &Y, sizeof(int)));
}
