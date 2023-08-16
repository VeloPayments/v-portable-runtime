/**
 * \file test_compare_unsigned_short.cpp
 *
 * Unit tests for compare_unsigned_short.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_unsigned_short);

/**
 * Test that comparing two unsigned shorts that are equal results in 0.
 */
TEST(equality)
{
    const unsigned short X = 17;
    const unsigned short Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(unsigned short)));
    TEST_EXPECT(0 == compare_unsigned_short(&X, &Y, sizeof(unsigned short)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const unsigned short X = 17;
    const unsigned short Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(unsigned short)));
    TEST_EXPECT(0 < compare_unsigned_short(&X, &Y, sizeof(unsigned short)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const unsigned short X = 17;
    const unsigned short Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(unsigned short)));
    TEST_EXPECT(0 > compare_unsigned_short(&X, &Y, sizeof(unsigned short)));
}
