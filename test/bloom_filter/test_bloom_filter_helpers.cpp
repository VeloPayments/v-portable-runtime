/**
 * \file test_bloom_filter_helpers.cpp
 *
 * Unit tests for bloom filter helper functions.
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/bloom_filter.h>

TEST_SUITE(bloom_filter_helpers);

/**
 * Test that we can calculate the size of a bloom filter.
 */
TEST(calculate_size_test)
{
    TEST_ASSERT(bloom_filter_calculate_size(10, 0.5) == (size_t)2);

    TEST_ASSERT(bloom_filter_calculate_size(4000, 0.1) == (size_t)2397);

    TEST_ASSERT(bloom_filter_calculate_size(10000, 0.1) == (size_t)5991);

    TEST_ASSERT(bloom_filter_calculate_size(100000, 0.05) == (size_t)77941);

    TEST_ASSERT(bloom_filter_calculate_size(10000000, 0.03) == (size_t)9123052);
}

/**
 * Test that we can calculate the number of hash functions that should be used.
 */
TEST(calculate_num_hashes)
{
    TEST_ASSERT(bloom_filter_calculate_num_hashes(10, 2) == 1u);

    TEST_ASSERT(bloom_filter_calculate_num_hashes(4000, 2397) == 3u);

    TEST_ASSERT(bloom_filter_calculate_num_hashes(10000, 5991) == 3u);

    TEST_ASSERT(bloom_filter_calculate_num_hashes(100000, 77941) == 4u);

    TEST_ASSERT(bloom_filter_calculate_num_hashes(10000000, 9123052) == 5u);
}

/**
 * Test that we can calculate the expected error rate of a bloom filter.
 */
TEST(calculate_expected_error_rate)
{
    float r = bloom_filter_calculate_expected_error_rate(10, 2, 1);
    TEST_ASSERT(r >= 0.464);
    TEST_ASSERT(r <= 0.465);

    r = bloom_filter_calculate_expected_error_rate(4000, 2397, 3);
    TEST_ASSERT(r >= 0.100);
    TEST_ASSERT(r <= 0.101);

    r = bloom_filter_calculate_expected_error_rate(10000, 5991, 3);
    TEST_ASSERT(r >= 0.100);
    TEST_ASSERT(r <= 0.101);

    r = bloom_filter_calculate_expected_error_rate(100000, 77941, 4);
    TEST_ASSERT(r >= 0.050);
    TEST_ASSERT(r <= 0.051);

    r = bloom_filter_calculate_expected_error_rate(10000000, 9123052, 5);
    TEST_ASSERT(r >= 0.030);
    TEST_ASSERT(r <= 0.031);
}
