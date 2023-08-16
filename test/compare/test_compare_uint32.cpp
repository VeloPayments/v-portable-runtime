/**
 * \file test_compare_uint32.cpp
 *
 * Unit tests for compare_uint32.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_uint32);

/**
 * Test that comparing two uint32_t values that are equal results in 0.
 */
TEST(equality)
{
    const uint32_t X = 17;
    const uint32_t Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(uint32_t)));
    TEST_EXPECT(0 == compare_uint32(&X, &Y, sizeof(uint32_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const uint32_t X = 17;
    const uint32_t Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(uint32_t)));
    TEST_EXPECT(0 < compare_uint32(&X, &Y, sizeof(uint32_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const uint32_t X = 17;
    const uint32_t Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(uint32_t)));
    TEST_EXPECT(0 > compare_uint32(&X, &Y, sizeof(uint32_t)));
}
