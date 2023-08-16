/**
 * \file test_compare_int64.cpp
 *
 * Unit tests for compare_int64.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_int64);

/**
 * Test that comparing two int64_t values that are equal results in 0.
 */
TEST(equality)
{
    const int64_t X = 17;
    const int64_t Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(int64_t)));
    TEST_EXPECT(0 == compare_int64(&X, &Y, sizeof(int64_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const int64_t X = 17;
    const int64_t Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(int64_t)));
    TEST_EXPECT(0 < compare_int64(&X, &Y, sizeof(int64_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const int64_t X = 17;
    const int64_t Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(int64_t)));
    TEST_EXPECT(0 > compare_int64(&X, &Y, sizeof(int64_t)));
}
