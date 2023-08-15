/**
 * \file test_compare_bool.cpp
 *
 * Unit tests for compare_bool.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0
/**
 * Test that comparing two boolean values that are equal results in 0.
 */
TEST(compare_bool, equality_true)
{
    const bool X = true;
    const bool Y = true;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(bool)));
    EXPECT_EQ(0, compare_bool(&X, &Y, sizeof(bool)));
}

/**
 * Test that comparing two boolean values that are equal results in 0.
 */
TEST(compare_bool, equality_false)
{
    const bool X = false;
    const bool Y = false;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(bool)));
    EXPECT_EQ(0, compare_bool(&X, &Y, sizeof(bool)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_bool, greater_than)
{
    const bool X = true;
    const bool Y = false;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(bool)));
    EXPECT_LT(0, compare_bool(&X, &Y, sizeof(bool)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_bool, less_than)
{
    const bool X = false;
    const bool Y = true;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(bool)));
    EXPECT_GT(0, compare_bool(&X, &Y, sizeof(bool)));
}
#endif
