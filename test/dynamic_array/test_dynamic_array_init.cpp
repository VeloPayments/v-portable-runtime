/**
 * \file test_dynamic_array_init.cpp
 *
 * Unit tests for dynamic_array_init.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_init_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        dynamic_array_options_init(
            &options, &alloc_opts, sizeof(int), &compare_int);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

TEST_F(dynamic_array_init_test, basic_test)
{
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //create an array of ints that is 10 elements in size all initialized with
    //our copy value.
    EXPECT_EQ(0, dynamic_array_init(&options, &array, 10, 10, &SEVENTEEN));

    //iterate over the array, checking that each value matches
    const int* intarr = (const int*)array.array;
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(SEVENTEEN, intarr[i]);
    }

    //dispose of our array
    dispose((disposable_t*)&array);
}
