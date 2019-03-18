/**
 * \file test_doubly_linked_list_insert_after.cpp
 *
 * Unit tests for doubly_linked_list_insert_after
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

class DLLInsertAfterTest : public ::testing::Test {
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

TEST_F(DLLInsertAfterTest, BasicTest)
{
    SetUp(true);

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
    EXPECT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);
    EXPECT_EQ(dll.elements, 2UL);
    EXPECT_EQ(*(int*)(dll.first->data), data);
    EXPECT_EQ(*(int*)(dll.last->data), data2);

    // test the links on each element
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, dll.last);
    EXPECT_EQ(dll.last->prev, dll.first);
    EXPECT_EQ(dll.last->next, nullptr);


    //dispose of our list
    dispose((disposable_t*)&dll);
}

TEST_F(DLLInsertAfterTest, WithCopyOnInsert)
{
    SetUp(true);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // insert something after the initial element
    int data2 = 205;
    EXPECT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(int*)(dll.first->data), data - 1);

    data2 += 10;
    EXPECT_EQ(*(int*)(dll.last->data), data2 - 10);


    //dispose of our list
    dispose((disposable_t*)&dll);
}

TEST_F(DLLInsertAfterTest, WithOutCopyOnInsert)
{
    SetUp(false);

    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    int data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // insert something after the initial element
    int data2 = 205;
    EXPECT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(int*)(dll.first->data), data);

    data2 += 10;
    EXPECT_EQ(*(int*)(dll.last->data), data2);


    //dispose of our list
    dispose((disposable_t*)&dll);
}
