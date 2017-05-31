/**
 * \file test_dynamic_array_sort.cpp
 *
 * Unit tests for dynamic_array_sort.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_sort_test : public ::testing::Test {
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
 * Create a dynamic array, add elements to it, and sort it.
 */
TEST_F(dynamic_array_sort_test, basic_test)
{
    int unsorted_values[] = { 7, 3, 9, 4, 10, 2, 5, 1, 8, 6 };
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    dynamic_array_t array;
    dynamic_array_init(&options, &array, 10, 0, NULL);

    //we should have a reserved space of 10
    ASSERT_EQ((size_t)10, array.reserved_elements);
    //there should be no element instances
    ASSERT_EQ((size_t)0, array.elements);

    //append 10 values to the array
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(0, dynamic_array_append(&array, unsorted_values + i));
    }

    //the reserved size should still be 10
    EXPECT_EQ((size_t)10, array.reserved_elements);
    //the instance size should be 10
    EXPECT_EQ((size_t)10, array.elements);

    //sorting should return 0
    ASSERT_EQ(0, dynamic_array_sort(&array));

    //the 10 values in the array should be sorted
    int* intArr = (int*)array.array;
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(sorted_values[i], intArr[i]);
    }

    //dispose the array
    dispose((disposable_t*)&array);
}
