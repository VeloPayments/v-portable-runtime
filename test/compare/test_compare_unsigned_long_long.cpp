/**
 * \file test_compare_unsigned_long_long.cpp
 *
 * Unit tests for compare_unsigned_long_long.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_unsigned_long_long);

/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(equality)
{
    const unsigned long long X = 17;
    const unsigned long long Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(unsigned long long)));
    TEST_EXPECT(
        0 == compare_unsigned_long_long(&X, &Y, sizeof(unsigned long long)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const unsigned long long X = 17;
    const unsigned long long Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(unsigned long long)));
    TEST_EXPECT(
        0 < compare_unsigned_long_long(&X, &Y, sizeof(unsigned long long)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const unsigned long long X = 17;
    const unsigned long long Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(unsigned long long)));
    TEST_EXPECT(
        0 > compare_unsigned_long_long(&X, &Y, sizeof(unsigned long long)));
}
