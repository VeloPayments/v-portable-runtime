/**
 * \file test_linked_list_insert_beginning.cpp
 *
 * Unit tests for linked_list_insert_beginning
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

class ll_insert_beginning_test : public ::testing::Test {
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
            dispose((disposable_t*)&options);
        }
        dispose((disposable_t*)&alloc_opts);
    }

    int linked_list_options_init_status;
    allocator_options_t alloc_opts;
    linked_list_options_t options;
};

TEST_F(ll_insert_beginning_test, options_init)
{
    LocalSetUp(true);
    ASSERT_EQ(VPR_STATUS_SUCCESS, linked_list_options_init_status);
}

TEST_F(ll_insert_beginning_test, basic_test)
{
    LocalSetUp(true);

    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    int data = 356;

    ASSERT_EQ(linked_list_insert_beginning(&ll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    EXPECT_EQ(ll.elements, 1UL);
    EXPECT_EQ(ll.first, ll.last);

    // the pointer on the initial element should be set to NULL
    EXPECT_EQ(ll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(int*)(ll.first->data), data);

    // insert something new at the beginning
    int data2 = 205;
    ASSERT_EQ(linked_list_insert_beginning(&ll, &data2), 0);

    // the number of elements should be 2, with the first element being the
    // second data item and the last element being the first data item
    EXPECT_EQ(ll.elements, 2UL);
    EXPECT_EQ(*(int*)(ll.first->data), data2);
    EXPECT_EQ(*(int*)(ll.last->data), data);

    // test the links on each element
    EXPECT_EQ(ll.first->next, ll.last);
    EXPECT_EQ(ll.last->next, nullptr);

    //dispose of our list
    dispose((disposable_t*)&ll);
}

TEST_F(ll_insert_beginning_test, with_copy_on_insert)
{
    LocalSetUp(true);

    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(linked_list_insert_beginning(&ll, &data), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(int*)(ll.first->data), data - 1);

    //dispose of our list
    dispose((disposable_t*)&ll);
}

TEST_F(ll_insert_beginning_test, without_copy_on_insert)
{
    LocalSetUp(false);

    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(linked_list_insert_beginning(&ll, &data), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(int*)(ll.first->data), data);

    //dispose of our list
    dispose((disposable_t*)&ll);
}
