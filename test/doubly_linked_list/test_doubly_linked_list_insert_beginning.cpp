/**
 * \file test_doubly_linked_list_insert_beginning.cpp
 *
 * Unit tests for doubly_linked_list_insert_beginning
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

class DLLInsertBeginningTest : public ::testing::Test {
protected:
    void SetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init(&options, &alloc_opts,
            copy_on_insert, sizeof(int), false);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_F(DLLInsertBeginningTest, BasicTest)
{
    SetUp(true);

    doubly_linked_list dll;

    EXPECT_EQ(doubly_linked_list_init(&options, &dll), 0);

    int data = 356;

    EXPECT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    EXPECT_EQ(dll.elements, 1UL);
    EXPECT_EQ(dll.first, dll.last);

    // the pointers on the initial element should be set to NULL
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(int*)(dll.first->data), data);

    // insert something new at the beginning
    int data2 = 205;
    EXPECT_EQ(doubly_linked_list_insert_beginning(&dll, &data2), 0);

    // the number of elements should be 2, with the first element being the
    // second data item and the last element being the first data item
    EXPECT_EQ(dll.elements, 2UL);
    EXPECT_EQ(*(long*)(dll.first->data), data2);
    EXPECT_EQ(*(long*)(dll.last->data), data);

    // test the links on each element
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, dll.last);
    EXPECT_EQ(dll.last->prev, dll.first);
    EXPECT_EQ(dll.last->next, nullptr);

    //dispose of our list
    dispose((disposable_t*)&dll);
}

TEST_F(DLLInsertBeginningTest, WithCopyOnInsert)
{
    SetUp(true);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(int*)(dll.first->data), data - 1);

    //dispose of our list
    dispose((disposable_t*)&dll);
}

TEST_F(DLLInsertBeginningTest, WithOutCopyOnInsert)
{
    SetUp(false);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(int*)(dll.first->data), data);

    //dispose of our list
    dispose((disposable_t*)&dll);
}
