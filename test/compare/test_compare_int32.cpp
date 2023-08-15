/**
 * \file test_compare_int32.cpp
 *
 * Unit tests for compare_int32.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0
/**
 * Test that comparing two int32_t values that are equal results in 0.
 */
TEST(compare_int32, equality)
{
    const int32_t X = 17;
    const int32_t Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(int32_t)));
    EXPECT_EQ(0, compare_int32(&X, &Y, sizeof(int32_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_int32, greater_than)
{
    const int32_t X = 17;
    const int32_t Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(int32_t)));
    EXPECT_LT(0, compare_int32(&X, &Y, sizeof(int32_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_int32, less_than)
{
    const int32_t X = 17;
    const int32_t Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(int32_t)));
    EXPECT_GT(0, compare_int32(&X, &Y, sizeof(int32_t)));
}
#endif
