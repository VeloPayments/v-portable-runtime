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

class doubly_linked_list_insert_beginning_test : public ::testing::Test {
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

TEST_F(doubly_linked_list_insert_beginning_test, basic_test)
{
    doubly_linked_list dll;

    EXPECT_EQ(doubly_linked_list_init(&options, &dll), 0);

    int data = 356;
    doubly_linked_list_element_t element;
    element.data = &data;

    EXPECT_EQ(doubly_linked_list_insert_beginning(&dll, &element), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    EXPECT_EQ(dll.elements, 1UL);
    EXPECT_EQ(dll.first, &element);
    EXPECT_EQ(dll.last, &element);

    // the pointers on the initial element should be set to NULL
    EXPECT_EQ(element.prev, nullptr);
    EXPECT_EQ(element.next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(int*)(dll.first->data), 356);

    // insert something new at the beginning
    int data2 = 205;
    doubly_linked_list_element_t element2;
    element2.data = &data2;

    EXPECT_EQ(doubly_linked_list_insert_beginning(&dll, &element2), 0);

    // the number of elements should be 2, with the first element
    // pointing to this element and the last element pointing to the previous element
    EXPECT_EQ(dll.elements, 2UL);
    EXPECT_EQ(dll.first, &element2);
    EXPECT_EQ(dll.last, &element);

    // test the links on each element
    EXPECT_EQ(element2.prev, nullptr);
    EXPECT_EQ(element2.next, &element);
    EXPECT_EQ(element.prev, &element2);
    EXPECT_EQ(element.next, nullptr);

    //dispose of our list
    dispose((disposable_t*)&dll);
}
