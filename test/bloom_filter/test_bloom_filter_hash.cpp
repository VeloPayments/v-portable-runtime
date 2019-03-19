/**
 * \file test_bloom_filter_init.cpp
 *
 * Unit tests for bloom_filter_init.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/bloom_filter.h>

class bloom_filter_hash_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init(&options, &alloc_opts, 8);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
};

TEST_F(bloom_filter_hash_test, basic_test)
{
    //    int n = 0;
    //    int m = 8;

    //    uint64_t hash_val = bloom_filter_hash(&options, n, m);

    //    EXPECT_NE(hash_val, (uint64_t)0);

    // test that the hash value changes with n, and is < m
}

TEST_F(bloom_filter_hash_test, hash_func1_distribution_test)
{
    // idea: test the average hamming distance between 1000 random inputs
}

TEST_F(bloom_filter_hash_test, hash_func2_distribution_test)
{
    // idea: test the average hamming distance between 1000 random inputs
}
