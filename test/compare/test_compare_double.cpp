/**
 * \file test_compare_double.cpp
 *
 * Unit tests for compare_double.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_double);

/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(equality)
{
    const double X = 17.0;
    const double Y = 17.0;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(double)));
    TEST_EXPECT(0 == compare_double(&X, &Y, sizeof(double)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const double X = 17.0;
    const double Y = 14.0;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(double)));
    TEST_EXPECT(0 < compare_double(&X, &Y, sizeof(double)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const double X = 17.0;
    const double Y = 19.0;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(double)));
    TEST_EXPECT(0 > compare_double(&X, &Y, sizeof(double)));
}
