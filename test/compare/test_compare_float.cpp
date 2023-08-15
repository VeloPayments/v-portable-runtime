/**
 * \file test_compare_float.cpp
 *
 * Unit tests for compare_float.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0
/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(compare_float, equality)
{
    const float X = 17.0;
    const float Y = 17.0;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(float)));
    EXPECT_EQ(0, compare_float(&X, &Y, sizeof(float)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_float, greater_than)
{
    const float X = 17.0;
    const float Y = 14.0;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(float)));
    EXPECT_LT(0, compare_float(&X, &Y, sizeof(float)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_float, less_than)
{
    const float X = 17.0;
    const float Y = 19.0;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(float)));
    EXPECT_GT(0, compare_float(&X, &Y, sizeof(float)));
}
#endif
