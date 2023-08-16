/**
 * \file test_compare_unsigned_short.cpp
 *
 * Unit tests for compare_unsigned_short.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0

/**
 * Test that comparing two unsigned shorts that are equal results in 0.
 */
TEST(compare_unsigned_short, equality)
{
    const unsigned short X = 17;
    const unsigned short Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(unsigned short)));
    EXPECT_EQ(0, compare_unsigned_short(&X, &Y, sizeof(unsigned short)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_unsigned_short, greater_than)
{
    const unsigned short X = 17;
    const unsigned short Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(unsigned short)));
    EXPECT_LT(0, compare_unsigned_short(&X, &Y, sizeof(unsigned short)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_unsigned_short, less_than)
{
    const unsigned short X = 17;
    const unsigned short Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(unsigned short)));
    EXPECT_GT(0, compare_unsigned_short(&X, &Y, sizeof(unsigned short)));
}
#endif
