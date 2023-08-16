/**
 * \file test_compare_int32.cpp
 *
 * Unit tests for compare_int32.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_int32);

/**
 * Test that comparing two int32_t values that are equal results in 0.
 */
TEST(equality)
{
    const int32_t X = 17;
    const int32_t Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(int32_t)));
    TEST_EXPECT(0 == compare_int32(&X, &Y, sizeof(int32_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const int32_t X = 17;
    const int32_t Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(int32_t)));
    TEST_EXPECT(0 < compare_int32(&X, &Y, sizeof(int32_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const int32_t X = 17;
    const int32_t Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(int32_t)));
    TEST_EXPECT(0 > compare_int32(&X, &Y, sizeof(int32_t)));
}
