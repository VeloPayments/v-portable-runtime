/**
 * \file test_bloom_filter.cpp
 *
 * Unit tests for bloom_filter.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/bloom_filter.h>

class bloom_filter_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init(&options, &alloc_opts, 4, 4);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
};

TEST_F(bloom_filter_test, init_test)
{
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)4);
    EXPECT_NE(bloom.bitmap, nullptr);

    // verify the bitmap is initialized to all 0s
    char testblock[bloom.options->size_in_bytes];
    memset(testblock, 0, bloom.options->size_in_bytes);
    EXPECT_EQ(
        memcmp(testblock, bloom.bitmap, bloom.options->size_in_bytes), 0);

    //dispose of our list
    dispose((disposable_t*)&bloom);
}

TEST_F(bloom_filter_test, simple_add_item_test)
{
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    const char* data = "add me to the bloom filter!";

    // initially that is NOT in the filter
    EXPECT_FALSE(bloom_filter_contains_item(&bloom, data));


    ASSERT_EQ(bloom_filter_add_item(&bloom, data), 0);

    // now the item should be in the filter
    // (no false positives possible when its the only item!)
    EXPECT_TRUE(bloom_filter_contains_item(&bloom, data));

    //dispose of our list
    dispose((disposable_t*)&bloom);
}
