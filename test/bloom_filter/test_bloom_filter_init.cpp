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

class bloom_filter_init_test : public ::testing::Test {
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

TEST_F(bloom_filter_init_test, basic_test)
{
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    EXPECT_EQ(bloom.options->size, (size_t)8);
    EXPECT_NE(bloom.bitmap, nullptr);

    // verify the bitmap is initialized to all 0s
    char testblock[bloom.options->size];
    memset(testblock, 0, bloom.options->size);
    EXPECT_EQ(memcmp(testblock, bloom.bitmap, bloom.options->size), 0);


    //dispose of our list
    dispose((disposable_t*)&bloom);
}
