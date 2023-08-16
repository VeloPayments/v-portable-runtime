/**
 * \file test_doubly_linked_list_insert_after.cpp
 *
 * Unit tests for doubly_linked_list_insert_after
 *
 * \copyright 2019-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

/* DISABLED GTEST */
#if 0
class dll_insert_after_test : public ::testing::Test {
protected:
    void LocalSetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init_status =
            doubly_linked_list_options_init(&options, &alloc_opts,
                copy_on_insert, sizeof(int), false);
    }

    void TearDown() override
    {
        if (VPR_STATUS_SUCCESS == doubly_linked_list_options_init_status)
        {
            dispose(doubly_linked_list_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int doubly_linked_list_options_init_status;
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_F(dll_insert_after_test, basic_test)
{
    LocalSetUp(true);

    /* verify that options init succeeded. */
    ASSERT_EQ(VPR_STATUS_SUCCESS, doubly_linked_list_options_init_status);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing to the same element
    EXPECT_EQ(dll.elements, 1UL);
    EXPECT_EQ(dll.first, dll.last);

    // the next / prev pointers on the initial element should be set to NULL
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(int*)(dll.first->data), data);

    // insert something after the initial element
    int data2 = 205;
    ASSERT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);
    EXPECT_EQ(dll.elements, 2UL);
    EXPECT_EQ(*(int*)(dll.first->data), data);
    EXPECT_EQ(*(int*)(dll.last->data), data2);

    // test the links on each element
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, dll.last);
    EXPECT_EQ(dll.last->prev, dll.first);
    EXPECT_EQ(dll.last->next, nullptr);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
}

TEST_F(dll_insert_after_test, with_copy_on_insert)
{
    LocalSetUp(true);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // insert something after the initial element
    int data2 = 205;
    ASSERT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(int*)(dll.first->data), data - 1);

    data2 += 10;
    EXPECT_EQ(*(int*)(dll.last->data), data2 - 10);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
}

TEST_F(dll_insert_after_test, without_copy_on_insert)
{
    LocalSetUp(false);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // insert something after the initial element
    int data2 = 205;
    ASSERT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(int*)(dll.first->data), data);

    data2 += 10;
    EXPECT_EQ(*(int*)(dll.last->data), data2);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
}
#endif
