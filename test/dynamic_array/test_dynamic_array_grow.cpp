/**
 * \file test_dynamic_array_grow.cpp
 *
 * Unit tests for dynamic_array_grow.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_grow_test : public ::testing::Test {
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
 * Create an empty array, and grow the reserved size by 1.
 */
TEST_F(dynamic_array_grow_test, basic_test)
{
    dynamic_array_t array;
    dynamic_array_init(&options, &array, 1, 0, NULL);

    //we should have a reserved space of 1
    ASSERT_EQ((size_t)1, array.reserved_elements);
    //there should be no element instances
    ASSERT_EQ((size_t)0, array.elements);

    //grow the array by 1
    EXPECT_EQ(0, dynamic_array_grow(&array, 2));

    //the reserved size should be 2
    EXPECT_EQ((size_t)2, array.reserved_elements);
    //the instance size should still be 0
    EXPECT_EQ((size_t)0, array.elements);

    //dispose the array
    dispose((disposable_t*)&array);
}

/**
 * Attempting to shrink the array should fail.
 */
TEST_F(dynamic_array_grow_test, shrink_failure)
{
    dynamic_array_t array;
    dynamic_array_init(&options, &array, 5, 0, NULL);

    //we should have a reserved space of 5
    ASSERT_EQ((size_t)5, array.reserved_elements);
    //there should be no element instances
    ASSERT_EQ((size_t)0, array.elements);

    //Attempt to shrink the array to 2 elements.  This should fail.
    ASSERT_NE(0, dynamic_array_grow(&array, 2));

    //the reserved size should still be 5
    EXPECT_EQ((size_t)5, array.reserved_elements);
    //the instance size should still be 0
    EXPECT_EQ((size_t)0, array.elements);

    //dispose the array
    dispose((disposable_t*)&array);
}

/**
 * Growing the array should copy instantianted elements.
 */
TEST_F(dynamic_array_grow_test, grow_copy)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;
    dynamic_array_init(&options, &array, 5, 5, &SEVENTEEN);

    //we should have a reserved space of 5
    ASSERT_EQ((size_t)5, array.reserved_elements);
    //All five elements should be instantiated
    ASSERT_EQ((size_t)5, array.elements);

    //all elements should be set
    int* intArr = (int*)array.array;
    for (int i = 0; i < 5; ++i)
    {
        ASSERT_EQ(SEVENTEEN, intArr[i]);
    }

    //Grow the array to 100 elements.
    ASSERT_EQ(0, dynamic_array_grow(&array, 100));

    //the reserved size should be 100
    EXPECT_EQ((size_t)100, array.reserved_elements);
    //the instance size should still be 5
    EXPECT_EQ((size_t)5, array.elements);
    //all elements should be set
    intArr = (int*)array.array;
    for (int i = 0; i < 5; ++i)
    {
        ASSERT_EQ(SEVENTEEN, intArr[i]);
    }

    //dispose the array
    dispose((disposable_t*)&array);
}
