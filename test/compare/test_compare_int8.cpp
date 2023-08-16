/**
 * \file test_compare_int8.cpp
 *
 * Unit tests for compare_int8.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_int8);

/**
 * Test that comparing two int8_t values that are equal results in 0.
 */
TEST(equality)
{
    const int8_t X = 17;
    const int8_t Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(int8_t)));
    TEST_EXPECT(0 == compare_int8(&X, &Y, sizeof(int8_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const int8_t X = 17;
    const int8_t Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(int8_t)));
    TEST_EXPECT(0 < compare_int8(&X, &Y, sizeof(int8_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const int8_t X = 17;
    const int8_t Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(int8_t)));
    TEST_EXPECT(0 > compare_int8(&X, &Y, sizeof(int8_t)));
}
