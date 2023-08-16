/**
 * \file test_compare_unsigned_int.cpp
 *
 * Unit tests for compare_unsigned_int.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_unsigned_int);

/**
 * Test that comparing two unsigned ints that are equal results in 0.
 */
TEST(equality)
{
    const unsigned int X = 17;
    const unsigned int Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(unsigned int)));
    TEST_EXPECT(0 == compare_unsigned_int(&X, &Y, sizeof(unsigned int)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const unsigned int X = 17;
    const unsigned int Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(unsigned int)));
    TEST_EXPECT(0 < compare_unsigned_int(&X, &Y, sizeof(unsigned int)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const unsigned int X = 17;
    const unsigned int Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(unsigned int)));
    TEST_EXPECT(0 > compare_unsigned_int(&X, &Y, sizeof(unsigned int)));
}
