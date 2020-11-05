/**
 * \file test_hashmap.cpp
 *
 * Unit tests for hashmap.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <stdbool.h>
#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

// forward decls
static bool always_equal(const void* lhs, const void* rhs);
static bool never_equal(const void* lhs, const void* rhs);

class hashmap_test : public ::testing::Test {
protected:
    void LocalSetUp(uint32_t capacity, hashmap_value_equals_t equals_func,
        bool copy_on_put, size_t val_size)
    {
        malloc_allocator_options_init(&alloc_opts);
        hashmap_options_init_status =
            hashmap_options_init(&options, &alloc_opts, capacity, equals_func,
                copy_on_put, val_size, false);
    }

    void TearDown() override
    {
        if (VPR_STATUS_SUCCESS == hashmap_options_init_status)
        {
            dispose((disposable_t*)&options);
        }
        dispose((disposable_t*)&alloc_opts);
    }

    int hashmap_options_init_status;
    allocator_options_t alloc_opts;
    hashmap_options_t options;
};

/**
 * Test that hashmap_options_init was successful.
 */
TEST_F(hashmap_test, options_init)
{
    LocalSetUp(1000, NULL, false, sizeof(int));
    ASSERT_EQ(VPR_STATUS_SUCCESS, hashmap_options_init_status);
}

/**
 * Test that the hashmap is initialized properly.
 */
TEST_F(hashmap_test, init_test)
{
    // set up a hashmap with a capacity of 1000
    LocalSetUp(1000, NULL, false, sizeof(int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    ASSERT_EQ(hmap.options->capacity, 1000u);

    // verify the hashmap is initialized to all NULLs
    ASSERT_EQ(*(uint8_t**)hmap.buckets, nullptr);
    for (unsigned int i = 0; i < hmap.options->capacity; i++)
    {
        uint8_t* ptr = *((uint8_t**)((uint8_t*)hmap.buckets + i));
        EXPECT_EQ(ptr, nullptr);
    }

    // there should be 0 elements
    EXPECT_EQ(hmap.elements, 0u);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test a simple PUT and GET operation without copy-on-put
 */
TEST_F(hashmap_test, put_get_without_copy)
{
    LocalSetUp(1024, NULL, false, sizeof(int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    uint64_t key = (uint64_t)1337;

    // should be nothing in the bucket that key is mapped to
    EXPECT_EQ(hashmap_get64(&hmap, key), nullptr);

    // add the value to the hashmap
    int val = 99;
    ASSERT_EQ(hashmap_put64(&hmap, key, &val), 0);

    // we should have one element now
    EXPECT_EQ(hmap.elements, 1u);

    // get our value back out
    void* ptr = hashmap_get64(&hmap, key);
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*(int*)ptr, val);

    // as we did not copy-on-insert the value should
    // reference our stack variable
    ++val;
    EXPECT_EQ(*(int*)hashmap_get64(&hmap, key), val);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test PUT/GET with variable length key
 */
TEST_F(hashmap_test, put_get_var_key)
{
    LocalSetUp(1024, NULL, false, sizeof(long));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    uint8_t key[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    size_t key_len = sizeof(key) / sizeof(uint8_t);

    // should be nothing in the bucket the key is mapped to
    EXPECT_EQ(hashmap_get(&hmap, key, key_len), nullptr);

    // add the value to the hashmap
    long val = 9999L;
    ASSERT_EQ(hashmap_put(&hmap, key, key_len, &val), 0);

    // we should have one element now
    EXPECT_EQ(hmap.elements, 1u);

    // GET the value back out and compare
    long* found = (long*)hashmap_get(&hmap, key, key_len);
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(*found, val);

    // dispose of our hashmap
    dispose((disposable_t*)&hmap);
}


/**
 * Test that the user supplied equality function is triggered.
 */
TEST_F(hashmap_test, equality_function)
{
    LocalSetUp(1024, &never_equal, false, sizeof(long));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    uint64_t key = (uint64_t)1337;

    // should be nothing in the bucket that key is mapped to
    EXPECT_EQ(hashmap_get64(&hmap, key), nullptr);

    // add the value to the hashmap
    int val = 99;
    ASSERT_EQ(hashmap_put64(&hmap, key, &val), 0);

    // we should have one element now
    EXPECT_EQ(hmap.elements, 1u);

    // because the equality check always says "false", we shouldn't find the
    // value
    void* ptr = hashmap_get64(&hmap, key);
    EXPECT_EQ(ptr, nullptr);

    // now replace the equality check and try again
    options.equals_func = &always_equal;
    ptr = hashmap_get64(&hmap, key);
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*(int*)ptr, val);

    // verify a hashed key match is required by querying for a non-existing value
    EXPECT_EQ(hashmap_get64(&hmap, key + 1), nullptr);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}


/**
 * Test a simple PUT and GET operation using a complex
 * structure with copy-on-put
 */
TEST_F(hashmap_test, put_get_with_copy)
{
    typedef struct test_type
    {
        int a;
        long b;
        _Bool c;
    } test_type_t;

    LocalSetUp(1024, NULL, true, sizeof(test_type_t));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    uint64_t key = (uint64_t)123;

    // should be nothing in the bucket that key is mapped to
    EXPECT_EQ(hashmap_get64(&hmap, key), nullptr);

    // add the value to the hashmap
    test_type_t val;
    val.a = 123;
    val.b = 456L;
    val.c = true;
    ASSERT_EQ(hashmap_put64(&hmap, key, &val), 0);

    // we should have one element now
    EXPECT_EQ(hmap.elements, 1u);

    // get our value back out
    void* ptr = hashmap_get64(&hmap, key);
    EXPECT_NE(ptr, nullptr);
    test_type_t* found = (test_type_t*)ptr;
    EXPECT_EQ(found->a, val.a);
    EXPECT_EQ(found->b, val.b);
    EXPECT_EQ(found->c, val.c);

    // as we did copy-on-insert the value should not reference our stack
    // variable
    ++val.a;
    --val.b;
    found = (test_type_t*)hashmap_get64(&hmap, key);

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
    LocalSetUp(2, NULL, false, sizeof(int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    // add the value to the hashmap
    int val0 = 100;
    ASSERT_EQ(hashmap_put64(&hmap, 0, &val0), 0);
    EXPECT_EQ(hmap.elements, 1u);

    // add a new value
    int val1 = 101;
    ASSERT_EQ(hashmap_put64(&hmap, 1, &val1), 0);
    EXPECT_EQ(hmap.elements, 2u);

    // this entry overwrites the first entry
    int val2 = 102;
    ASSERT_EQ(hashmap_put64(&hmap, 0, &val2), 0);
    EXPECT_EQ(hmap.elements, 2u);
    int* found = (int*)hashmap_get64(&hmap, 0);
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
    LocalSetUp(capacity, NULL, true, sizeof(unsigned int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    // add 10 times more items than we have capacity for
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        hashmap_put64(&hmap, key, &i);
    }
    EXPECT_EQ(hmap.elements, capacity * 10);

    // retrieve each of those items and ensure we have the correct value
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        unsigned int* val = (unsigned int*)hashmap_get64(&hmap, key);
        EXPECT_EQ(*val, i);
    }

    // add the values again, overwriting the originals
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        hashmap_put64(&hmap, key, &i);
    }
    EXPECT_EQ(hmap.elements, capacity * 10);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

/**
 * Test add all values with same key
 */
TEST_F(hashmap_test, duplicate_key)
{
    unsigned int capacity = 100;
    LocalSetUp(capacity, NULL, true, sizeof(unsigned int));
    hashmap hmap;

    ASSERT_EQ(hashmap_init(&options, &hmap), 0);

    // add 10 times more items than we have capacity for
    uint64_t key = 333;
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        hashmap_put64(&hmap, key, &i);

        // read the value back out
        unsigned int* found = (unsigned int*)hashmap_get64(&hmap, key);
        EXPECT_EQ(*found, i);
    }

    // we should have just one item
    EXPECT_EQ(hmap.elements, 1u);

    //dispose of our hashmap
    dispose((disposable_t*)&hmap);
}

static bool always_equal(const void* UNUSED(lhs), const void* UNUSED(rhs))
{
    return true;
}

static bool never_equal(const void* UNUSED(lhs), const void* UNUSED(rhs))
{
    return false;
}
