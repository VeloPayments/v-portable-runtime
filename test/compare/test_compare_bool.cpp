/**
 * \file test_compare_bool.cpp
 *
 * Unit tests for compare_bool.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(compare_bool);

/**
 * Test that comparing two boolean values that are equal results in 0.
 */
TEST(equality_true)
{
    const bool X = true;
    const bool Y = true;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(bool)));
    TEST_EXPECT(0 == compare_bool(&X, &Y, sizeof(bool)));
}

/**
 * Test that comparing two boolean values that are equal results in 0.
 */
TEST(equality_false)
{
    const bool X = false;
    const bool Y = false;

    TEST_EXPECT(0 == memcmp(&X, &Y, sizeof(bool)));
    TEST_EXPECT(0 == compare_bool(&X, &Y, sizeof(bool)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(greater_than)
{
    const bool X = true;
    const bool Y = false;

    TEST_EXPECT(0 < memcmp(&X, &Y, sizeof(bool)));
    TEST_EXPECT(0 < compare_bool(&X, &Y, sizeof(bool)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(less_than)
{
    const bool X = false;
    const bool Y = true;

    TEST_EXPECT(0 > memcmp(&X, &Y, sizeof(bool)));
    TEST_EXPECT(0 > compare_bool(&X, &Y, sizeof(bool)));
}
