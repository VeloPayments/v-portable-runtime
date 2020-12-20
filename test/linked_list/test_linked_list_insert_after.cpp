/**
 * \file test_linked_list_insert_after.cpp
 *
 * Unit tests for linked_list_insert_after
 *
 * \copyright 2019-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

class ll_insert_after_test : public ::testing::Test {
protected:
    void LocalSetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        linked_list_options_init_status =
            linked_list_options_init(&options, &alloc_opts,
                copy_on_insert, sizeof(int), false);
    }

    void TearDown() override
    {
        if (VPR_STATUS_SUCCESS == linked_list_options_init_status)
        {
            dispose(linked_list_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int linked_list_options_init_status;
    allocator_options_t alloc_opts;
    linked_list_options_t options;
};

TEST_F(ll_insert_after_test, options_init)
{
    LocalSetUp(true);
    ASSERT_EQ(VPR_STATUS_SUCCESS, linked_list_options_init_status);
}

TEST_F(ll_insert_after_test, basic_test)
{
    LocalSetUp(true);

    linked_list ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(linked_list_insert_end(&ll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing to the same element
    EXPECT_EQ(ll.elements, 1UL);
    EXPECT_EQ(ll.first, ll.last);

    // the next pointer on the initial element should be set to NULL
    EXPECT_EQ(ll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(int*)(ll.first->data), data);

    // insert something after the initial element
    int data2 = 205;
    ASSERT_EQ(linked_list_insert_after(&ll, ll.first, &data2), 0);
    EXPECT_EQ(ll.elements, 2UL);
    EXPECT_EQ(*(int*)(ll.first->data), data);
    EXPECT_EQ(*(int*)(ll.last->data), data2);

    // test the next links on each element
    EXPECT_EQ(ll.first->next, ll.last);
    EXPECT_EQ(ll.last->next, nullptr);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
}

TEST_F(ll_insert_after_test, with_copy_on_insert)
{
    LocalSetUp(true);

    linked_list ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(linked_list_insert_end(&ll, &data), 0);

    // insert something after the initial element
    int data2 = 205;
    ASSERT_EQ(linked_list_insert_after(&ll, ll.first, &data2), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(int*)(ll.first->data), data - 1);

    data2 += 10;
    EXPECT_EQ(*(int*)(ll.last->data), data2 - 10);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
}

TEST_F(ll_insert_after_test, without_copy_on_insert)
{
    LocalSetUp(false);

    linked_list ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(linked_list_insert_end(&ll, &data), 0);

    // insert something after the initial element
    int data2 = 205;
    ASSERT_EQ(linked_list_insert_after(&ll, ll.first, &data2), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(int*)(ll.first->data), data);

    data2 += 10;
    EXPECT_EQ(*(int*)(ll.last->data), data2);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
}
