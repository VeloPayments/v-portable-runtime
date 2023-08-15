/**
 * \file test_dynamic_array_init.cpp
 *
 * Unit tests for dynamic_array_init.
 *
 * \copyright 2017-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

/* DISABLED GTEST */
#if 0
class dynamic_array_init_test : public ::testing::Test {
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

TEST_F(dynamic_array_init_test, options_init)
{
    ASSERT_EQ(VPR_STATUS_SUCCESS, dynamic_array_options_init_status);
}

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
    dispose(dynamic_array_disposable_handle(&array));
}
#endif
