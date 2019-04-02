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
        hashmap_options_init(&options, &alloc_opts, capacity,
            false, sizeof(int), false);
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
    for (unsigned int i = 0; i < hmap.options->capacity; i++)
    {
        uint8_t* ptr = i + (uint8_t*)hmap.buckets;
        EXPECT_EQ(*ptr, 0);
    }

    // there should be 0 elements
    EXPECT_EQ(hmap.elements, 0u);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test a simple PUT and GET operation
 */
TEST_F(hashmap_test, simple_put_test)
{
    SetUp(1024);
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    uint64_t key = (uint64_t)1337;

    // should be nothing in the bucket that key is mapped to
    EXPECT_EQ(hashmap_get(&hmap, key), nullptr);

    // add the value to the hashmap
    int val = 99;
    ASSERT_EQ(hashmap_put(&hmap, key, &val), 0);

    // we should have one element now
    EXPECT_EQ(hmap.elements, 1u);

    // get our value back out
    void* ptr = hashmap_get(&hmap, key);
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*(int*)ptr, val);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}
