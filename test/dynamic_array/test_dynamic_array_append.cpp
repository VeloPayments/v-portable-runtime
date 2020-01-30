/**
 * \file test_dynamic_array_append.cpp
 *
 * Unit tests for dynamic_array_append.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_append_test : public ::testing::Test {
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
            dispose((disposable_t*)&options);
        }
        dispose((disposable_t*)&alloc_opts);
    }

    int dynamic_array_options_init_status;
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

/**
 * dynamic_array_options_init should succeed.
 */
TEST_F(dynamic_array_append_test, options_init)
{
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_options_init_status);
}

/**
 * Create an empty array and append an element to it.
 */
TEST_F(dynamic_array_append_test, basic_test)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //this should succeed
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 1, 0, NULL));

    //we should have a reserved space of 1
    ASSERT_EQ((size_t)1, array.reserved_elements);
    //there should be no element instances
    ASSERT_EQ((size_t)0, array.elements);

    //append a value to the array
    EXPECT_EQ(0, dynamic_array_append(&array, &SEVENTEEN));

    //the reserved size should still be 1
    EXPECT_EQ((size_t)1, array.reserved_elements);
    //the instance size should be 1
    EXPECT_EQ((size_t)1, array.elements);

    //the first element in the array should be set to our value
    ASSERT_NE((void*)NULL, (void*)array.array);
    int* intArray = (int*)array.array;
    EXPECT_EQ(SEVENTEEN, intArray[0]);

    //dispose the array
    dispose((disposable_t*)&array);
}

/**
 * Appending should fail if the array is full, but should succeed if the array
 * is grown.
 */
TEST_F(dynamic_array_append_test, full_array)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //this should succeed
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        dynamic_array_init(&options, &array, 1, 1, &SEVENTEEN));

    //we should have a reserved space of 1
    ASSERT_EQ((size_t)1, array.reserved_elements);
    //there should be one element instance
    ASSERT_EQ((size_t)1, array.elements);

    //appending fails
    ASSERT_NE(0, dynamic_array_append(&array, &SEVENTEEN));

    //growing succeeds
    ASSERT_EQ(0, dynamic_array_grow(&array, 2));

    //now, appending succeeds
    ASSERT_EQ(0, dynamic_array_append(&array, &SEVENTEEN));

    //the reserved size should be 2
    EXPECT_EQ((size_t)2, array.reserved_elements);
    //the instance size should be 2
    EXPECT_EQ((size_t)2, array.elements);

    //the first two elements in the array should be set
    ASSERT_NE((void*)NULL, (void*)array.array);
    int* intArray = (int*)array.array;
    EXPECT_EQ(SEVENTEEN, intArray[0]);
    EXPECT_EQ(SEVENTEEN, intArray[1]);

    //dispose the array
    dispose((disposable_t*)&array);
}
