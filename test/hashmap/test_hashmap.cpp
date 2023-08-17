/**
 * \file test_hashmap.cpp
 *
 * Unit tests for hashmap.
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <stdbool.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

// forward decls
static bool always_equal(const void* lhs, const void* rhs);
static bool never_equal(const void* lhs, const void* rhs);

class hashmap_test {
public:
    void localSetUp(uint32_t capacity, hashmap_value_equals_t equals_func,
        bool copy_on_put, size_t val_size)
    {
        malloc_allocator_options_init(&alloc_opts);
        hashmap_options_init_status =
            hashmap_options_init(&options, &alloc_opts, capacity, equals_func,
                copy_on_put, val_size, false);
    }

    void tearDown()
    {
        if (VPR_STATUS_SUCCESS == hashmap_options_init_status)
        {
            dispose(hashmap_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int hashmap_options_init_status;
    allocator_options_t alloc_opts;
    hashmap_options_t options;
};

TEST_SUITE(hashmap_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    hashmap_test fixture;

#define END_TEST_F() \
    fixture.tearDown(); \
}

/**
 * Test that hashmap_options_init was successful.
 */
BEGIN_TEST_F(options_init)
    fixture.localSetUp(1000, NULL, false, sizeof(int));
    TEST_ASSERT(VPR_STATUS_SUCCESS == fixture.hashmap_options_init_status);
END_TEST_F()

/**
 * Test that the hashmap is initialized properly.
 */
BEGIN_TEST_F(init_test)
    // set up a hashmap with a capacity of 1000
    fixture.localSetUp(1000, NULL, false, sizeof(int));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    TEST_ASSERT(hmap.options->capacity == 1000u);

    // verify the hashmap is initialized to all NULLs
    TEST_ASSERT(*(uint8_t**)hmap.buckets == nullptr);
    for (unsigned int i = 0; i < hmap.options->capacity; i++)
    {
        uint8_t* ptr = *((uint8_t**)((uint8_t*)hmap.buckets + i));
        TEST_EXPECT(ptr == nullptr);
    }

    // there should be 0 elements
    TEST_EXPECT(hmap.elements == 0u);

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()

/**
 * Test a simple PUT and GET operation without copy-on-put
 */
BEGIN_TEST_F(put_get_without_copy)
    fixture.localSetUp(1024, NULL, false, sizeof(int));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    uint64_t key = (uint64_t)1337;

    // should be nothing in the bucket that key is mapped to
    TEST_EXPECT(hashmap_get64(&hmap, key) == nullptr);

    // add the value to the hashmap
    int val = 99;
    TEST_ASSERT(hashmap_put64(&hmap, key, &val) == 0);

    // we should have one element now
    TEST_EXPECT(hmap.elements == 1u);

    // get our value back out
    void* ptr = hashmap_get64(&hmap, key);
    TEST_EXPECT(ptr != nullptr);
    TEST_EXPECT(*(int*)ptr == val);

    // as we did not copy-on-insert the value should
    // reference our stack variable
    ++val;
    TEST_EXPECT(*(int*)hashmap_get64(&hmap, key) == val);

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()

/**
 * Test PUT/GET with variable length key
 */
BEGIN_TEST_F(put_get_var_key)
    fixture.localSetUp(1024, NULL, false, sizeof(long));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    uint8_t key[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    size_t key_len = sizeof(key) / sizeof(uint8_t);

    // should be nothing in the bucket the key is mapped to
    TEST_EXPECT(hashmap_get(&hmap, key, key_len) == nullptr);

    // add the value to the hashmap
    long val = 9999L;
    TEST_ASSERT(hashmap_put(&hmap, key, key_len, &val) == 0);

    // we should have one element now
    TEST_EXPECT(hmap.elements == 1u);

    // GET the value back out and compare
    long* found = (long*)hashmap_get(&hmap, key, key_len);
    TEST_EXPECT(found != nullptr);
    TEST_EXPECT(*found == val);

    // dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()


/**
 * Test that the user supplied equality function is triggered.
 */
BEGIN_TEST_F(equality_function)
    fixture.localSetUp(1024, &never_equal, false, sizeof(long));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    uint64_t key = (uint64_t)1337;

    // should be nothing in the bucket that key is mapped to
    TEST_EXPECT(hashmap_get64(&hmap, key) == nullptr);

    // add the value to the hashmap
    int val = 99;
    TEST_ASSERT(hashmap_put64(&hmap, key, &val) == 0);

    // we should have one element now
    TEST_EXPECT(hmap.elements == 1u);

    // because the equality check always says "false", we shouldn't find the
    // value
    void* ptr = hashmap_get64(&hmap, key);
    TEST_EXPECT(ptr == nullptr);

    // now replace the equality check and try again
    fixture.options.equals_func = &always_equal;
    ptr = hashmap_get64(&hmap, key);
    TEST_EXPECT(ptr != nullptr);
    TEST_EXPECT(*(int*)ptr == val);

    // verify a hashed key match is required by querying for a non-existing value
    TEST_EXPECT(hashmap_get64(&hmap, key + 1) == nullptr);

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()


/**
 * Test a simple PUT and GET operation using a complex
 * structure with copy-on-put
 */
BEGIN_TEST_F(put_get_with_copy)
    typedef struct test_type
    {
        int a;
        long b;
        _Bool c;
    } test_type_t;

    fixture.localSetUp(1024, NULL, true, sizeof(test_type_t));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    uint64_t key = (uint64_t)123;

    // should be nothing in the bucket that key is mapped to
    TEST_EXPECT(hashmap_get64(&hmap, key) == nullptr);

    // add the value to the hashmap
    test_type_t val;
    val.a = 123;
    val.b = 456L;
    val.c = true;
    TEST_ASSERT(hashmap_put64(&hmap, key, &val) == 0);

    // we should have one element now
    TEST_EXPECT(hmap.elements == 1u);

    // get our value back out
    void* ptr = hashmap_get64(&hmap, key);
    TEST_EXPECT(ptr != nullptr);
    test_type_t* found = (test_type_t*)ptr;
    TEST_EXPECT(found->a == val.a);
    TEST_EXPECT(found->b == val.b);
    TEST_EXPECT(found->c == val.c);

    // as we did copy-on-insert the value should not reference our stack
    // variable
    ++val.a;
    --val.b;
    found = (test_type_t*)hashmap_get64(&hmap, key);

    TEST_EXPECT(found->a == val.a - 1);
    TEST_EXPECT(found->b == val.b + 1);
    TEST_EXPECT(found->c == val.c);  // unchanged

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()

/**
 * Test adding multiple values to a very small map, some with the same key.
 */
BEGIN_TEST_F(add_multiple_items)
    fixture.localSetUp(2, NULL, false, sizeof(int));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    // add the value to the hashmap
    int val0 = 100;
    TEST_ASSERT(hashmap_put64(&hmap, 0, &val0) == 0);
    TEST_EXPECT(hmap.elements == 1u);

    // add a new value
    int val1 = 101;
    TEST_ASSERT(hashmap_put64(&hmap, 1, &val1) == 0);
    TEST_EXPECT(hmap.elements == 2u);

    // this entry overwrites the first entry
    int val2 = 102;
    TEST_ASSERT(hashmap_put64(&hmap, 0, &val2) == 0);
    TEST_EXPECT(hmap.elements == 2u);
    int* found = (int*)hashmap_get64(&hmap, 0);
    TEST_EXPECT(*found == val2);

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()


/**
 * Test overloading the hashmap with more values than buckets.  As long as
 * values are added with unique keys, the hashmap should continue to perform
 * correctly, though values will chained in each bucket.
 */
BEGIN_TEST_F(chaining_round_robin)
    unsigned int capacity = 100;
    fixture.localSetUp(capacity, NULL, true, sizeof(unsigned int));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    // add 10 times more items than we have capacity for
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        hashmap_put64(&hmap, key, &i);
    }
    TEST_EXPECT(hmap.elements == capacity * 10);

    // retrieve each of those items and ensure we have the correct value
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        unsigned int* val = (unsigned int*)hashmap_get64(&hmap, key);
        TEST_EXPECT(*val == i);
    }

    // add the values again, overwriting the originals
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        uint64_t key = i;
        hashmap_put64(&hmap, key, &i);
    }
    TEST_EXPECT(hmap.elements == capacity * 10);

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()

/**
 * Test add all values with same key
 */
BEGIN_TEST_F(duplicate_key)
    unsigned int capacity = 100;
    fixture.localSetUp(capacity, NULL, true, sizeof(unsigned int));
    hashmap hmap;

    TEST_ASSERT(hashmap_init(&fixture.options, &hmap) == 0);

    // add 10 times more items than we have capacity for
    uint64_t key = 333;
    for (unsigned int i = 0; i < (capacity * 10); i++)
    {
        hashmap_put64(&hmap, key, &i);

        // read the value back out
        unsigned int* found = (unsigned int*)hashmap_get64(&hmap, key);
        TEST_EXPECT(*found == i);
    }

    // we should have just one item
    TEST_EXPECT(hmap.elements == 1u);

    //dispose of our hashmap
    dispose(hashmap_disposable_handle(&hmap));
END_TEST_F()

static bool always_equal(const void* UNUSED(lhs), const void* UNUSED(rhs))
{
    return true;
}

static bool never_equal(const void* UNUSED(lhs), const void* UNUSED(rhs))
{
    return false;
}
