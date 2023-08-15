/**
 * \file test_compare_uint32.cpp
 *
 * Unit tests for compare_uint32.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/compare.h>

/* DISABLED GTEST */
#if 0
/**
 * Test that comparing two uint32_t values that are equal results in 0.
 */
TEST(compare_uint32, equality)
{
    const uint32_t X = 17;
    const uint32_t Y = 17;

    EXPECT_EQ(0, memcmp(&X, &Y, sizeof(uint32_t)));
    EXPECT_EQ(0, compare_uint32(&X, &Y, sizeof(uint32_t)));
}

/**
 * Test that X > Y results in a return value that is greater than zero.
 */
TEST(compare_uint32, greater_than)
{
    const uint32_t X = 17;
    const uint32_t Y = 14;

    EXPECT_LT(0, memcmp(&X, &Y, sizeof(uint32_t)));
    EXPECT_LT(0, compare_uint32(&X, &Y, sizeof(uint32_t)));
}

/**
 * Test that X < Y results in a return value that is less than zero.
 */
TEST(compare_uint32, less_than)
{
    const uint32_t X = 17;
    const uint32_t Y = 19;

    EXPECT_GT(0, memcmp(&X, &Y, sizeof(uint32_t)));
    EXPECT_GT(0, compare_uint32(&X, &Y, sizeof(uint32_t)));
}
#endif
