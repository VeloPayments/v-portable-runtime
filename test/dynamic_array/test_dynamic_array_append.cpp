/**
 * \file test_dynamic_array_append.cpp
 *
 * Unit tests for dynamic_array_append.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_append_test {
public:
    void setUp()
    {
        malloc_allocator_options_init(&alloc_opts);
        dynamic_array_options_init_status =
            dynamic_array_options_init(
                &options, &alloc_opts, sizeof(int), &compare_int);
    }

    void tearDown()
    {
        if (VPR_STATUS_SUCCESS == dynamic_array_options_init_status)
        {
            dispose(dynamic_array_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int dynamic_array_options_init_status;
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

TEST_SUITE(dynamic_array_append_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dynamic_array_append_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

/**
 * dynamic_array_options_init should succeed.
 */
BEGIN_TEST_F(options_init)
    TEST_ASSERT(
        VPR_STATUS_SUCCESS == fixture.dynamic_array_options_init_status);
END_TEST_F()

/**
 * Create an empty array and append an element to it.
 */
BEGIN_TEST_F(basic_test)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //this should succeed
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 0, NULL));

    //we should have a reserved space of 1
    TEST_ASSERT((size_t)1 == array.reserved_elements);
    //there should be no element instances
    TEST_ASSERT((size_t)0 == array.elements);

    //append a value to the array
    TEST_EXPECT(0 == dynamic_array_append(&array, &SEVENTEEN));

    //the reserved size should still be 1
    TEST_EXPECT((size_t)1 == array.reserved_elements);
    //the instance size should be 1
    TEST_EXPECT((size_t)1 == array.elements);

    //the first element in the array should be set to our value
    TEST_ASSERT((void*)NULL != (void*)array.array);
    int* intArray = (int*)array.array;
    TEST_EXPECT(SEVENTEEN == intArray[0]);

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Appending should fail if the array is full, but should succeed if the array
 * is grown.
 */
BEGIN_TEST_F(full_array)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //this should succeed
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 1, &SEVENTEEN));

    //we should have a reserved space of 1
    TEST_ASSERT((size_t)1 == array.reserved_elements);
    //there should be one element instance
    TEST_ASSERT((size_t)1 == array.elements);

    //appending fails
    TEST_ASSERT(0 != dynamic_array_append(&array, &SEVENTEEN));

    //growing succeeds
    TEST_ASSERT(0 == dynamic_array_grow(&array, 2));

    //now, appending succeeds
    TEST_ASSERT(0 == dynamic_array_append(&array, &SEVENTEEN));

    //the reserved size should be 2
    TEST_EXPECT((size_t)2 == array.reserved_elements);
    //the instance size should be 2
    TEST_EXPECT((size_t)2 == array.elements);

    //the first two elements in the array should be set
    TEST_ASSERT((void*)NULL != (void*)array.array);
    int* intArray = (int*)array.array;
    TEST_EXPECT(SEVENTEEN == intArray[0]);
    TEST_EXPECT(SEVENTEEN == intArray[1]);

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()
