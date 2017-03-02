/**
 * \file test_compare_double.cpp
 *
 * Unit tests for compare_double.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/compare.h>

/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(compare_double, equality)
{
    const double X = 17.0;
    const double Y = 17.0;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(double)));
    EXPECT_EQ(0, compare_double(&X, &Y, sizeof(double)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_double, greater_than)
{
    const double X = 17.0;
    const double Y = 14.0;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(double)));
    EXPECT_LT(0, compare_double(&X, &Y, sizeof(double)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_double, less_than)
{
    const double X = 17.0;
    const double Y = 19.0;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(double)));
    EXPECT_GT(0, compare_double(&X, &Y, sizeof(double)));
}
