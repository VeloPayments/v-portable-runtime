/**
 * \file test_doubly_linked_list_remove.cpp
 *
 * Unit tests for doubly_linked_list_remove
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

void build_doubly_linked_list(doubly_linked_list_t* dll, int* data, int n);


class dll_remove_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init(&options, &alloc_opts,
            false, sizeof(int), false);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_F(dll_remove_test, basic_test)
{
    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    int data_elements[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    build_doubly_linked_list(&dll, data_elements, 10);

    // there should be 10 elements, all in order
    EXPECT_EQ(dll.elements, 10UL);
    doubly_linked_list_element_t* element = dll.first;
    doubly_linked_list_element_t* element5 = NULL;
    int i = 0;
    while (element != NULL)
    {

        EXPECT_EQ(*(int*)element->data, data_elements[i]);
        if (i == 5)
        {
            element5 = element;
        }
        element = element->next;
        ++i;
    }
    EXPECT_EQ(i, 10);
    EXPECT_NE(element5, nullptr);

    // now remove the fifth element
    EXPECT_EQ(0, doubly_linked_list_remove(&dll, element5));
    EXPECT_EQ(dll.elements, 9UL);

    // traverse the list again, ensuring the removed element isn't present
    i = 0;
    element = dll.first;
    while (element != NULL)
    {
        EXPECT_NE(element, element5);
        element = element->next;
        ++i;
    }
    EXPECT_EQ(i, 9);

    // remove the last element
    ASSERT_EQ(0, doubly_linked_list_remove(&dll, dll.last));
    EXPECT_EQ(dll.elements, 8UL);
    EXPECT_EQ(*(int*)dll.last->data, 8);

    // remove the first element
    ASSERT_EQ(0, doubly_linked_list_remove(&dll, dll.first));
    EXPECT_EQ(dll.elements, 7UL);
    EXPECT_EQ(*(int*)dll.first->data, 1);

    // remove all remaining elements
    for (i = 0; i < 7; i++)
    {
        ASSERT_EQ(0, doubly_linked_list_remove(&dll, dll.first));
    }
    EXPECT_EQ(dll.elements, 0UL);
    EXPECT_EQ(dll.first, nullptr);
    EXPECT_EQ(dll.last, nullptr);
}

void build_doubly_linked_list(doubly_linked_list_t* dll, int* data, int n)
{
    ASSERT_NE(dll, nullptr);
    ASSERT_NE(data, nullptr);
    ASSERT_NE(n, 0);

    for (int i = 0; i < n; i++)
    {
        doubly_linked_list_insert_end(dll, &data[i]);
    }
}
