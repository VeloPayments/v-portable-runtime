/**
 * \file test_compare_short.cpp
 *
 * Unit tests for compare_short.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two shorts that are equal results in 0.
 */
TEST(compare_short, equality)
{
    const short X = 17;
    const short Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(short)));
    EXPECT_EQ(0, compare_short(&X, &Y, sizeof(short)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_short, greater_than)
{
    const short X = 17;
    const short Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(short)));
    EXPECT_LT(0, compare_short(&X, &Y, sizeof(short)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_short, less_than)
{
    const short X = 17;
    const short Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(short)));
    EXPECT_GT(0, compare_short(&X, &Y, sizeof(short)));
}
