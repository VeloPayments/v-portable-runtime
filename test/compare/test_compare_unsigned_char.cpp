/**
 * \file test_compare_unsigned_char.cpp
 *
 * Unit tests for compare_unsigned_char.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_unsigned_char);

/**
 * Test that comparing two unsigned characters that are equal results in 0.
 */
TEST(equality)
{
    const unsigned char X = 17;
    const unsigned char Y = 17;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(unsigned char)));
    TEST_EXPECT(0 == compare_unsigned_char(&X, &Y, sizeof(unsigned char)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const unsigned char X = 17;
    const unsigned char Y = 14;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(unsigned char)));
    TEST_EXPECT(0 < compare_unsigned_char(&X, &Y, sizeof(unsigned char)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const unsigned char X = 17;
    const unsigned char Y = 19;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(unsigned char)));
    TEST_EXPECT(0 > compare_unsigned_char(&X, &Y, sizeof(unsigned char)));
}
