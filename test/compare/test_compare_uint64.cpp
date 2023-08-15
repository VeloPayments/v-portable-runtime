/**
 * \file test_compare_uint64.cpp
 *
 * Unit tests for compare_uint64.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0

/**
 * Test that comparing two uint64_t values that are equal results in 0.
 */
TEST(compare_uint64, equality)
{
    const uint64_t X = 17;
    const uint64_t Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(uint64_t)));
    EXPECT_EQ(0, compare_uint64(&X, &Y, sizeof(uint64_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_uint64, greater_than)
{
    const uint64_t X = 17;
    const uint64_t Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(uint64_t)));
    EXPECT_LT(0, compare_uint64(&X, &Y, sizeof(uint64_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_uint64, less_than)
{
    const uint64_t X = 17;
    const uint64_t Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(uint64_t)));
    EXPECT_GT(0, compare_uint64(&X, &Y, sizeof(uint64_t)));
}
#endif
