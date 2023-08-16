/**
 * \file test_compare_long.cpp
 *
 * Unit tests for compare_long.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_long);

/**
 * Test that comparing two longs that are equal results in 0.
 */
TEST(equality)
{
    const long X = 17;
    const long Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(long)));
    TEST_EXPECT(0 == compare_long(&X, &Y, sizeof(long)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const long X = 17;
    const long Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(long)));
    TEST_EXPECT(0 < compare_long(&X, &Y, sizeof(long)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const long X = 17;
    const long Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(long)));
    TEST_EXPECT(0 > compare_long(&X, &Y, sizeof(long)));
}
