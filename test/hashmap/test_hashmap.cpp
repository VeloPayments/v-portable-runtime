/**
 * \file test_hashmap.cpp
 *
 * Unit tests for hashmap.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/hashmap.h>

class hashmap_test : public ::testing::Test {
protected:
    void SetUp(unsigned int capacity)
    {
        malloc_allocator_options_init(&alloc_opts);
        hashmap_options_init(&options, &alloc_opts, capacity);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    hashmap_options_t options;
};

/**
 * Test that the hashmap is initialized properly.
 */
TEST_F(hashmap_test, init_test)
{
    // set up a hashmap with a capacity of 1000
    SetUp(1000);
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    ASSERT_EQ(hmap.options->capacity, 1000u);

    // verify the hashmap is initialized to all NULLs
    void* testblock[hmap.options->capacity];
    memset(testblock, 0, hmap.options->capacity * sizeof(void*));
    EXPECT_EQ(
        memcmp(testblock, hmap.buckets,
            hmap.options->capacity * sizeof(void*)),
        0);

    // there should be 0 elements
    EXPECT_EQ(hmap.elements, 0u);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}
