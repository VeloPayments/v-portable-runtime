/**
 * \file test_compare_long_double.cpp
 *
 * Unit tests for compare_long_double.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_long_double);

/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(equality)
{
    const long double X = 17.0;
    const long double Y = 17.0;

    TEST_EXPECT(0 == compare_long_double(&X, &Y, sizeof(long double)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const long double X = 17.0;
    const long double Y = 14.0;

    TEST_EXPECT(0 < compare_long_double(&X, &Y, sizeof(long double)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const long double X = 17.0;
    const long double Y = 19.0;

    TEST_EXPECT(0 > compare_long_double(&X, &Y, sizeof(long double)));
}
