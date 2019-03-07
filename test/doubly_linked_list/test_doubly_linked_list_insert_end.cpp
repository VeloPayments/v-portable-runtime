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
        doubly_linked_list_options_init(&options, &alloc_opts);
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
    doubly_linked_list_element_t element;
    element.data = &data;

    EXPECT_EQ(doubly_linked_list_insert_end(&dll, &element), 0);

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
    EXPECT_EQ(*(int*)(dll.last->data), 356);

    // insert something new at the end
    int data2 = 205;
    doubly_linked_list_element_t element2;
    element2.data = &data2;

    EXPECT_EQ(doubly_linked_list_insert_end(&dll, &element2), 0);

    // the number of elements should be 2, with the last element
    // pointing to this element and the first element pointing to the previous element
    EXPECT_EQ(dll.elements, 2UL);
    EXPECT_EQ(dll.first, &element);
    EXPECT_EQ(dll.last, &element2);

    // test the links on each element
    EXPECT_EQ(element.prev, nullptr);
    EXPECT_EQ(element.next, &element2);
    EXPECT_EQ(element2.prev, &element);
    EXPECT_EQ(element2.next, nullptr);

    // insert a third element on the end
    int data3 = 301;
    doubly_linked_list_element_t element3;
    element3.data = &data3;

    EXPECT_EQ(doubly_linked_list_insert_end(&dll, &element3), 0);

    // should be three elements: e1 -> e2 -> e3
    EXPECT_EQ(dll.elements, 3UL);
    EXPECT_EQ(dll.first, &element);
    EXPECT_EQ(dll.last, &element3);

    EXPECT_EQ(element.prev, nullptr);
    EXPECT_EQ(element.next, &element2);
    EXPECT_EQ(element2.prev, &element);
    EXPECT_EQ(element2.next, &element3);
    EXPECT_EQ(element3.prev, &element2);
    EXPECT_EQ(element3.next, nullptr);

    //dispose of our list
    dispose((disposable_t*)&dll);
}
