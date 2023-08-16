/**
 * \file test_compare_int.cpp
 *
 * Unit tests for compare_int.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_int);

/**
 * Test that comparing two ints that are equal results in 0.
 */
TEST(equality)
{
    const int X = 17;
    const int Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(int)));
    TEST_EXPECT(0 == compare_int(&X, &Y, sizeof(int)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const int X = 17;
    const int Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(int)));
    TEST_EXPECT(0 < compare_int(&X, &Y, sizeof(int)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const int X = 17;
    const int Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(int)));
    TEST_EXPECT(0 > compare_int(&X, &Y, sizeof(int)));
}
