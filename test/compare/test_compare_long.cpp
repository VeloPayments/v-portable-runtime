/**
 * \file test_compare_long.cpp
 *
 * Unit tests for compare_long.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0

/**
 * Test that comparing two longs that are equal results in 0.
 */
TEST(compare_long, equality)
{
    const long X = 17;
    const long Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(long)));
    EXPECT_EQ(0, compare_long(&X, &Y, sizeof(long)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_long, greater_than)
{
    const long X = 17;
    const long Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(long)));
    EXPECT_LT(0, compare_long(&X, &Y, sizeof(long)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_long, less_than)
{
    const long X = 17;
    const long Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(long)));
    EXPECT_GT(0, compare_long(&X, &Y, sizeof(long)));
}
#endif
