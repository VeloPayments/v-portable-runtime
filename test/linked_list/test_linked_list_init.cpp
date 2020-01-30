/**
 * \file test_linked_list_init.cpp
 *
 * Unit tests for linked_list_init.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

class ll_init_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        linked_list_options_init_status =
            linked_list_options_init(&options, &alloc_opts,
                false, sizeof(int), false);
    }

    void TearDown() override
    {
        if (VPR_STATUS_SUCCESS == linked_list_options_init_status)
        {
            dispose((disposable_t*)&options);
        }
        dispose((disposable_t*)&alloc_opts);
    }

    int linked_list_options_init_status;
    allocator_options_t alloc_opts;
    linked_list_options_t options;
};

TEST_F(ll_init_test, options_init)
{
    ASSERT_EQ(VPR_STATUS_SUCCESS, linked_list_options_init_status);
}

TEST_F(ll_init_test, basic_test)
{
    linked_list ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // the number of elements should be initialized to 0, with
    // the first and last pointers pointing to NULL
    EXPECT_EQ(ll.elements, 0UL);
    EXPECT_EQ(ll.first, nullptr);
    EXPECT_EQ(ll.last, nullptr);

    //dispose of our list
    dispose((disposable_t*)&ll);
}
