/**
 * \file test_dynamic_array_grow.cpp
 *
 * Unit tests for dynamic_array_grow.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_grow_test {
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

TEST_SUITE(dynamic_array_grow_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dynamic_array_grow_test fixture; \
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
 * Create an empty array, and grow the reserved size by 1.
 */
BEGIN_TEST_F(basic_test)
    dynamic_array_t array;

    //we should be able to create a dynamic array
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 0, NULL));

    //we should have a reserved space of 1
    TEST_ASSERT((size_t)1 == array.reserved_elements);
    //there should be no element instances
    TEST_ASSERT((size_t)0 == array.elements);

    //grow the array by 1
    TEST_EXPECT(0 == dynamic_array_grow(&array, 2));

    //the reserved size should be 2
    TEST_EXPECT((size_t)2 == array.reserved_elements);
    //the instance size should still be 0
    TEST_EXPECT((size_t)0 == array.elements);

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Attempting to shrink the array should fail.
 */
BEGIN_TEST_F(shrink_failure)
    dynamic_array_t array;

    //we should be able to create a dynamic array
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 0, NULL));

    //we should have a reserved space of 5
    TEST_ASSERT((size_t)5 == array.reserved_elements);
    //there should be no element instances
    TEST_ASSERT((size_t)0 == array.elements);

    //Attempt to shrink the array to 2 elements.  This should fail.
    TEST_ASSERT(0 != dynamic_array_grow(&array, 2));

    //the reserved size should still be 5
    TEST_EXPECT((size_t)5 == array.reserved_elements);
    //the instance size should still be 0
    TEST_EXPECT((size_t)0 == array.elements);

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Growing the array should copy instantianted elements.
 */
BEGIN_TEST_F(grow_copy)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //we should be able to create a dynamic array
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 5, &SEVENTEEN));

    //we should have a reserved space of 5
    TEST_ASSERT((size_t)5 == array.reserved_elements);
    //All five elements should be instantiated
    TEST_ASSERT((size_t)5 == array.elements);

    //all elements should be set
    int* intArr = (int*)array.array;
    for (int i = 0; i < 5; ++i)
    {
        TEST_ASSERT(SEVENTEEN == intArr[i]);
    }

    //Grow the array to 100 elements.
    TEST_ASSERT(0 == dynamic_array_grow(&array, 100));

    //the reserved size should be 100
    TEST_EXPECT((size_t)100 == array.reserved_elements);
    //the instance size should still be 5
    TEST_EXPECT((size_t)5 == array.elements);
    //all elements should be set
    intArr = (int*)array.array;
    for (int i = 0; i < 5; ++i)
    {
        TEST_ASSERT(SEVENTEEN == intArr[i]);
    }

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()
