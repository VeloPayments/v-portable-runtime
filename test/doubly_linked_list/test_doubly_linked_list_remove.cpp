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

static int build_doubly_linked_list(
    doubly_linked_list_t* dll, int* data, int n);


class dll_remove_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init_success =
            doubly_linked_list_options_init(&options, &alloc_opts,
                false, sizeof(int), false);
    }

    void TearDown() override
    {
        if (VPR_STATUS_SUCCESS == doubly_linked_list_options_init_success)
        {
            dispose((disposable_t*)&options);
        }
        dispose((disposable_t*)&alloc_opts);
    }

    int doubly_linked_list_options_init_success;
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_F(dll_remove_test, options_init)
{
    ASSERT_EQ(VPR_STATUS_SUCCESS, doubly_linked_list_options_init_success);
}

TEST_F(dll_remove_test, basic_test)
{
    doubly_linked_list_t dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    int data_elements[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    ASSERT_EQ(0, build_doubly_linked_list(&dll, data_elements, 10));

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
    doubly_linked_list_remove(&dll, element5);
    release(&alloc_opts, element5);
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
    doubly_linked_list_element_t* lastptr = dll.last;
    doubly_linked_list_remove(&dll, dll.last);
    release(&alloc_opts, lastptr);
    EXPECT_EQ(dll.elements, 8UL);
    EXPECT_EQ(*(int*)dll.last->data, 8);

    // remove the first element
    doubly_linked_list_element_t* firstptr = dll.first;
    doubly_linked_list_remove(&dll, dll.first);
    release(&alloc_opts, firstptr);
    EXPECT_EQ(dll.elements, 7UL);
    EXPECT_EQ(*(int*)dll.first->data, 1);

    // remove all remaining elements
    for (i = 0; i < 7; i++)
    {
        doubly_linked_list_element_t* curr = dll.first;
        doubly_linked_list_remove(&dll, dll.first);
        release(&alloc_opts, curr);
    }
    EXPECT_EQ(dll.elements, 0UL);
    EXPECT_EQ(dll.first, nullptr);
    EXPECT_EQ(dll.last, nullptr);

    //dispose of our list
    dispose((disposable_t*)&dll);
}

static int build_doubly_linked_list(
    doubly_linked_list_t* dll, int* data, int n)
{
    if (nullptr == dll)
        return -1;
    if (nullptr == data)
        return -1;
    if (0 == n)
        return -1;

    for (int i = 0; i < n; i++)
    {
        int retval = doubly_linked_list_insert_end(dll, &data[i]);
        if (VPR_STATUS_SUCCESS != retval)
            return retval;
    }

    return 0;
}
