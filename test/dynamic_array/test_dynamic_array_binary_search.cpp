/**
 * \file test_dynamic_array_binary_search.cpp
 *
 * Unit tests for dynamic_array_binary_search.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_binary_search_test
{
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

TEST_SUITE(dynamic_array_binary_search_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dynamic_array_binary_search_test fixture; \
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

    /* we should be able to create an array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 0, NULL));

    /* there should be no element instances. */
    TEST_ASSERT((size_t)0 == array.elements);

    /* search for the element. */
    TEST_EXPECT(NULL == dynamic_array_binary_search(&array, &SEVENTEEN));

    /* dispose of the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds.
 */
BEGIN_TEST_F(matching_element)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* create the array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 1, 1, &SEVENTEEN));

    /* there should be one element instance. */
    TEST_ASSERT((size_t)1 == array.elements);

    /* search for the element. */
    int* result = (int*)dynamic_array_binary_search(&array, &SEVENTEEN);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(SEVENTEEN == *result);

    /* dispose of the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is the first
 * element.
 */
BEGIN_TEST_F(first_matching_element)
    int SIXTEEN = 16;
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* create an array of five elements. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 5, &SEVENTEEN));

    /* there should be five element instances. */
    TEST_ASSERT((size_t)5 == array.elements);

    /* set the first element to SIXTEEN. */
    int* arr = (int*)array.array;
    arr[0] = SIXTEEN;

    /* search for the element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SIXTEEN);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(SIXTEEN == *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is not the first
 * element.
 */
BEGIN_TEST_F(last_matching_element)
    int SIXTEEN = 16;
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* we should be able to create an array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 4, &SIXTEEN));

    /* there should be four element instances. */
    TEST_ASSERT((size_t)4 == array.elements);

    /* append the matching element. */
    TEST_ASSERT(0 == dynamic_array_append(&array, &SEVENTEEN));

    /* search for the element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SEVENTEEN);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(SEVENTEEN == *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is the middle
 * element.
 */
BEGIN_TEST_F(middle_matching_element)
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    dynamic_array_t array;

    /* we should be able to create an array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 3, 0, NULL));

    /* append each element. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &ONE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &TWO));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &THREE));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &TWO);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(TWO == *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is near the
 * middle.
 */
BEGIN_TEST_F(near_middle_element)
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    int FOUR = 4;
    dynamic_array_t array;

    /* we should be able to create an array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 4, 0, NULL));

    /* append each element. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &ONE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &TWO));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &THREE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &FOUR));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &TWO);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(TWO == *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is near the
 * middle.
 */
BEGIN_TEST_F(near_middle_element2)
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    int FOUR = 4;
    int FIVE = 5;
    dynamic_array_t array;

    /* we should be able to create an array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 0, NULL));

    /* append each element. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &ONE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &TWO));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &THREE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &FOUR));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &FIVE));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &TWO);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(TWO == *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Searching an array with a matching element succeeds when it is near the
 * middle.
 */
BEGIN_TEST_F(near_middle_element3)
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    int FOUR = 4;
    int FIVE = 5;
    dynamic_array_t array;

    /* we should be able to create an array. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 5, 0, NULL));

    /* append each element. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &ONE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &TWO));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &THREE));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &FOUR));
    TEST_ASSERT(VPR_STATUS_SUCCESS == dynamic_array_append(&array, &FIVE));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &FOUR);
    TEST_ASSERT(nullptr != result);
    TEST_EXPECT(FOUR == *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()
