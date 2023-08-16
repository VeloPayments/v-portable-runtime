/**
 * \file test_compare_float.cpp
 *
 * Unit tests for compare_float.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_float);

/**
 * Test that comparing two long longs that are equal results in 0.
 */
TEST(equality)
{
    const float X = 17.0;
    const float Y = 17.0;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(float)));
    TEST_EXPECT(0 == compare_float(&X, &Y, sizeof(float)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const float X = 17.0;
    const float Y = 14.0;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(float)));
    TEST_EXPECT(0 < compare_float(&X, &Y, sizeof(float)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const float X = 17.0;
    const float Y = 19.0;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(float)));
    TEST_EXPECT(0 > compare_float(&X, &Y, sizeof(float)));
}
