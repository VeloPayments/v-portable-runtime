/**
 * \file test_dynamic_array_binary_search.cpp
 *
 * Unit tests for dynamic_array_binary_search.
 *
 * \copyright 2017-2021 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_binary_search_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        dynamic_array_options_init_status =
            dynamic_array_options_init(
                &options, &alloc_opts, sizeof(int), &compare_int);
    }

    void TearDown() override
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

/**
 * dynamic_array_options_init should succeed.
 */
TEST_F(dynamic_array_binary_search_test, options_init)
{
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_options_init_status);
}

/**
 * Searching an empty array fails to find an element.
 */
TEST_F(dynamic_array_binary_search_test, empty_array)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* we should be able to create an array. */
    ASSERT_EQ(
        VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 1, 0, NULL));

    /* there should be no element instances. */
    ASSERT_EQ((size_t)0, array.elements);

    /* search for the element. */
    EXPECT_EQ(NULL, dynamic_array_binary_search(&array, &SEVENTEEN));

    /* dispose of the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds.
 */
TEST_F(dynamic_array_binary_search_test, matching_element)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* create the array. */
    ASSERT_EQ(
        VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 1, 1, &SEVENTEEN));

    /* there should be one element instance. */
    ASSERT_EQ((size_t)1, array.elements);

    /* search for the element. */
    int* result = (int*)dynamic_array_binary_search(&array, &SEVENTEEN);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(SEVENTEEN, *result);

    /* dispose of the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds when it is the first
 * element.
 */
TEST_F(dynamic_array_binary_search_test, first_matching_element)
{
    int SIXTEEN = 16;
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* create an array of five elements. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 5, 5, &SEVENTEEN));

    /* there should be five element instances. */
    ASSERT_EQ((size_t)5, array.elements);

    /* set the first element to SIXTEEN. */
    int* arr = (int*)array.array;
    arr[0] = SIXTEEN;

    /* search for the element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SIXTEEN);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(SIXTEEN, *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds when it is not the first
 * element.
 */
TEST_F(dynamic_array_binary_search_test, last_matching_element)
{
    int SIXTEEN = 16;
    int SEVENTEEN = 17;
    dynamic_array_t array;

    /* we should be able to create an array. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 5, 4, &SIXTEEN));

    /* there should be four element instances. */
    ASSERT_EQ((size_t)4, array.elements);

    /* append the matching element. */
    ASSERT_EQ(0, dynamic_array_append(&array, &SEVENTEEN));

    /* search for the element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SEVENTEEN);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(SEVENTEEN, *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds when it is the middle
 * element.
 */
TEST_F(dynamic_array_binary_search_test, middle_matching_element)
{
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    dynamic_array_t array;

    /* we should be able to create an array. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 3, 0, NULL));

    /* append each element. */
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &ONE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &TWO));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &THREE));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &TWO);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(TWO, *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds when it is near the
 * middle.
 */
TEST_F(dynamic_array_binary_search_test, near_middle_element)
{
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    int FOUR = 4;
    dynamic_array_t array;

    /* we should be able to create an array. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 4, 0, NULL));

    /* append each element. */
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &ONE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &TWO));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &THREE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &FOUR));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &TWO);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(TWO, *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds when it is near the
 * middle.
 */
TEST_F(dynamic_array_binary_search_test, near_middle_element2)
{
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    int FOUR = 4;
    int FIVE = 5;
    dynamic_array_t array;

    /* we should be able to create an array. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 5, 0, NULL));

    /* append each element. */
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &ONE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &TWO));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &THREE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &FOUR));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &FIVE));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &TWO);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(TWO, *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
}

/**
 * Searching an array with a matching element succeeds when it is near the
 * middle.
 */
TEST_F(dynamic_array_binary_search_test, near_middle_element3)
{
    int ONE = 1;
    int TWO = 2;
    int THREE = 3;
    int FOUR = 4;
    int FIVE = 5;
    dynamic_array_t array;

    /* we should be able to create an array. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 5, 0, NULL));

    /* append each element. */
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &ONE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &TWO));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &THREE));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &FOUR));
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_append(&array, &FIVE));

    /* search for the TWO element. */
    int* result = (int*)dynamic_array_linear_search(&array, 0, &FOUR);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(FOUR, *result);

    /* dispose the array. */
    dispose(dynamic_array_disposable_handle(&array));
}
