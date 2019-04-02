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
    void SetUp(unsigned int capacity, _Bool copy_on_insert, size_t item_size)
    {
        malloc_allocator_options_init(&alloc_opts);
        hashmap_options_init(&options, &alloc_opts, capacity,
            copy_on_insert, item_size, false);
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
    SetUp(1000, false, sizeof(int));
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
 * Test a simple PUT and GET operation without copy-on-insert
 */
TEST_F(hashmap_test, put_get_without_copy)
{
    SetUp(1024, false, sizeof(int));
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

    // as we did not copy-on-insert the value should
    // reference our stack variable
    val = 100;
    EXPECT_EQ(*(int*)hashmap_get(&hmap, key), val);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test a simple PUT and GET operation using a complex
 * structure with copy-on-insert
 */
TEST_F(hashmap_test, put_get_with_copy)
{
    typedef struct test_type
    {
        int a;
        long b;
        _Bool c;
    } test_type_t;

    SetUp(1024, true, sizeof(test_type_t));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    uint64_t key = (uint64_t)123;

    // should be nothing in the bucket that key is mapped to
    EXPECT_EQ(hashmap_get(&hmap, key), nullptr);

    // add the value to the hashmap
    test_type_t val;
    val.a = 123;
    val.b = 456L;
    val.c = true;
    ASSERT_EQ(hashmap_put(&hmap, key, &val), 0);

    // we should have one element now
    EXPECT_EQ(hmap.elements, 1u);

    // get our value back out
    void* ptr = hashmap_get(&hmap, key);
    EXPECT_NE(ptr, nullptr);
    test_type_t* found = (test_type_t*)ptr;
    EXPECT_EQ(found->a, val.a);
    EXPECT_EQ(found->b, val.b);
    EXPECT_EQ(found->c, val.c);

    // as we did copy-on-insert the value should not reference our stack
    // variable
    ++val.a;
    --val.b;
    found = (test_type_t*)hashmap_get(&hmap, key);

    EXPECT_EQ(found->a, val.a - 1);
    EXPECT_EQ(found->b, val.b + 1);
    EXPECT_EQ(found->c, val.c);  // unchanged

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test adding multiple values to a very small map, some with the same key.
 */
TEST_F(hashmap_test, add_multiple_items)
{
    SetUp(2, false, sizeof(int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    // add the value to the hashmap
    int val0 = 100;
    ASSERT_EQ(hashmap_put(&hmap, 0, &val0), 0);
    EXPECT_EQ(hmap.elements, 1u);

    // add a new value
    int val1 = 101;
    ASSERT_EQ(hashmap_put(&hmap, 1, &val1), 0);
    EXPECT_EQ(hmap.elements, 2u);

    // this entry overwrites the first entry
    int val2 = 102;
    ASSERT_EQ(hashmap_put(&hmap, 0, &val2), 0);
    EXPECT_EQ(hmap.elements, 2u);
    int* found = (int*)hashmap_get(&hmap, 0);
    EXPECT_EQ(*found, val2);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}


/**
 * Test overloading the hashmap with more values than buckets.  As long as
 * values are added with unique keys, the hashmap should continue to perform
 * correctly, though values will chained in each bucket.
 */
TEST_F(hashmap_test, chaining_round_robin)
{
    unsigned int capacity = 100;
    SetUp(capacity, true, sizeof(unsigned int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    // add 10 times more items than we have capacity for
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        hashmap_put(&hmap, key, &i);
    }
    EXPECT_EQ(hmap.elements, capacity * 10);

    // retrieve each of those items and ensure we have the correct value
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        unsigned int* val = (unsigned int*)hashmap_get(&hmap, key);
        EXPECT_EQ(*val, i);
    }

    // add the values again, overwriting the originals
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        hashmap_put(&hmap, key, &i);
    }
    EXPECT_EQ(hmap.elements, capacity * 10);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test add all elements with same key
 */
TEST_F(hashmap_test, duplicate_key)
{
    unsigned int capacity = 100;
    SetUp(capacity, true, sizeof(unsigned int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    // add 10 times more items than we have capacity for
    uint64_t key = 333;
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        hashmap_put(&hmap, key, &i);

        // read the value back out
        unsigned int* found = (unsigned int*)hashmap_get(&hmap, key);
        EXPECT_EQ(*found, i);
    }

    // we should have just one item
    EXPECT_EQ(hmap.elements, 1u);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}
