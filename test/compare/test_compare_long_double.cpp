/**
 * \file test_compare_long_double.cpp
 *
 * Unit tests for compare_long_double.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(compare_long_double, equality)
{
    const long double X = 17.0;
    const long double Y = 17.0;

    EXPECT_EQ(0, compare_long_double(&X, &Y, sizeof(long double)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_long_double, greater_than)
{
    const long double X = 17.0;
    const long double Y = 14.0;

    EXPECT_LT(0, compare_long_double(&X, &Y, sizeof(long double)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_long_double, less_than)
{
    const long double X = 17.0;
    const long double Y = 19.0;

    EXPECT_GT(0, compare_long_double(&X, &Y, sizeof(long double)));
}
