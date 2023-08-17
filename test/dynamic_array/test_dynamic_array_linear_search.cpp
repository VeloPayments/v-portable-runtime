/**
 * \file test_dynamic_array_linear_search.cpp
 *
 * Unit tests for dynamic_array_linear_search.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_linear_search_test {
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

TEST_SUITE(dynamic_array_linear_search_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dynamic_array_linear_search_test fixture; \
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
 * Searching an empty array fails to find an element.
 */
BEGIN_TEST_F(empty_array)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //we should be able to create an array
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 0, NULL));

    //there should be no element instances
    TEST_ASSERT((size_t)0 == array.elements);

    //search for the element
    TEST_EXPECT(NULL == dynamic_array_linear_search(&array, 0, &SEVENTEEN));

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds.
 */
BEGIN_TEST_F(matching_element)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //we should be able to create an array
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 1, &SEVENTEEN));

    //there should be one element instance
    TEST_ASSERT((size_t)1 == array.elements);

    //search for the element
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SEVENTEEN);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(SEVENTEEN == *result);

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is not the first
 * element.
 */
BEGIN_TEST_F(last_matching_element)
    int SEVENTEEN = 17;
    int SIXTEEN = 16;
    dynamic_array_t array;

    //we should be able to create an array
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 4, &SEVENTEEN));

    //there should be four element instances
    TEST_ASSERT((size_t)4 == array.elements);

    //append the matching element
    TEST_ASSERT(0 == dynamic_array_append(&array, &SIXTEEN));

    //search for the element
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SIXTEEN);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(SIXTEEN == *result);

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()
