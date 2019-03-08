/**
 * \file test_doubly_linked_list_insert_end.cpp
 *
 * Unit tests for doubly_linked_list_insert_end
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

class doubly_linked_list_insert_end_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init(&options, &alloc_opts,
            sizeof(int));
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_F(doubly_linked_list_insert_end_test, basic_test)
{
    doubly_linked_list dll;

    EXPECT_EQ(doubly_linked_list_init(&options, &dll), 0);

    int data = 356;
    EXPECT_EQ(doubly_linked_list_insert_end(&dll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    EXPECT_EQ(dll.elements, 1UL);
    EXPECT_EQ(dll.first, dll.last);

    // the pointers on the initial element should be set to NULL
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(int*)(dll.first->data), data);
    EXPECT_EQ(*(int*)(dll.last->data), data);

    // insert something new at the end
    int data2 = 205;
    EXPECT_EQ(doubly_linked_list_insert_end(&dll, &data2), 0);

    // the number of elements should be 2, with the first element being the
    // second data item and the last element being the first data item
    EXPECT_EQ(dll.elements, 2UL);
    EXPECT_EQ(*(long*)(dll.first->data), data);
    EXPECT_EQ(*(long*)(dll.last->data), data2);

    // test the links on each element
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, dll.last);
    EXPECT_EQ(dll.last->prev, dll.first);
    EXPECT_EQ(dll.last->next, nullptr);

    // insert a third element on the end
    int data3 = 301;
    EXPECT_EQ(doubly_linked_list_insert_end(&dll, &data3), 0);

    // should be three elements: data -> data2 -> data3
    EXPECT_EQ(dll.elements, 3UL);
    EXPECT_EQ(*(int*)(dll.first->data), data);
    EXPECT_EQ(*(int*)(dll.first->next->data), data2);
    EXPECT_EQ(*(int*)(dll.first->next->next->data), data3);

    // now in reverse
    EXPECT_EQ(*(int*)(dll.last->data), data3);
    EXPECT_EQ(*(int*)(dll.last->prev->data), data2);
    EXPECT_EQ(*(int*)(dll.last->prev->prev->data), data);

    // test some pointers
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next->next, dll.last);
    EXPECT_EQ(dll.first->next, dll.last->prev);
    EXPECT_EQ(dll.last->prev->prev, dll.first);
    EXPECT_EQ(dll.last->next, nullptr);

    //dispose of our list
    dispose((disposable_t*)&dll);
}
