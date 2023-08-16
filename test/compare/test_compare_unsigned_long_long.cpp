/**
 * \file test_compare_unsigned_long_long.cpp
 *
 * Unit tests for compare_unsigned_long_long.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0
/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(compare_unsigned_long_long, equality)
{
    const unsigned long long X = 17;
    const unsigned long long Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(unsigned long long)));
    EXPECT_EQ(0,
        compare_unsigned_long_long(&X, &Y, sizeof(unsigned long long)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_unsigned_long_long, greater_than)
{
    const unsigned long long X = 17;
    const unsigned long long Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(unsigned long long)));
    EXPECT_LT(0,
        compare_unsigned_long_long(&X, &Y, sizeof(unsigned long long)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_unsigned_long_long, less_than)
{
    const unsigned long long X = 17;
    const unsigned long long Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(unsigned long long)));
    EXPECT_GT(0,
        compare_unsigned_long_long(&X, &Y, sizeof(unsigned long long)));
}
#endif
