/**
 * \file test_dynamic_array_linear_search.cpp
 *
 * Unit tests for dynamic_array_linear_search.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_linear_search_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        dynamic_array_options_init(
            &options, &alloc_opts, sizeof(int), &compare_int);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

/**
 * Searching an empty array fails to find an element.
 */
TEST_F(dynamic_array_linear_search_test, empty_array)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;
    dynamic_array_init(&options, &array, 1, 0, NULL);

    //there should be no element instances
    ASSERT_EQ((size_t)0, array.elements);

    //search for the element
    EXPECT_EQ(NULL, dynamic_array_linear_search(&array, 0, &SEVENTEEN));

    //dispose the array
    dispose((disposable_t*)&array);
}

/**
 * Searching an array with a matching element succeeds.
 */
TEST_F(dynamic_array_linear_search_test, matching_element)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;
    dynamic_array_init(&options, &array, 1, 1, &SEVENTEEN);

    //there should be one element instance
    ASSERT_EQ((size_t)1, array.elements);

    //search for the element
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SEVENTEEN);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(SEVENTEEN, *result);

    //dispose the array
    dispose((disposable_t*)&array);
}

/**
 * Searching an array with a matching element succeeds when it is not the first
 * element.
 */
TEST_F(dynamic_array_linear_search_test, last_matching_element)
{
    int SEVENTEEN = 17;
    int SIXTEEN = 16;
    dynamic_array_t array;
    dynamic_array_init(&options, &array, 5, 4, &SEVENTEEN);

    //there should be four element instances
    ASSERT_EQ((size_t)4, array.elements);

    //append the matching element
    ASSERT_EQ(0, dynamic_array_append(&array, &SIXTEEN));

    //search for the element
    int* result = (int*)dynamic_array_linear_search(&array, 0, &SIXTEEN);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(SIXTEEN, *result);

    //dispose the array
    dispose((disposable_t*)&array);
}
