/**
 * \file test_linked_list_remove.cpp
 *
 * Unit tests for linked_list_remove
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

void build_linked_list(linked_list_t* ll, int* data, int n);


class ll_remove_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        linked_list_options_init_status =
            linked_list_options_init(&options, &alloc_opts,
                false, sizeof(int), false);
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

TEST_F(ll_remove_test, options_init)
{
    ASSERT_EQ(VPR_STATUS_SUCCESS, linked_list_options_init_status);
}

TEST_F(ll_remove_test, basic_test)
{
    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    int data_elements[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    build_linked_list(&ll, data_elements, 10);

    // there should be 10 elements, all in order
    EXPECT_EQ(ll.elements, 10UL);
    linked_list_element_t* element = ll.first;
    linked_list_element_t* element5 = NULL;
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
    linked_list_remove(&ll, element5);
    release(&alloc_opts, element5);
    EXPECT_EQ(ll.elements, 9UL);

    // traverse the list again, ensuring the removed element isn't present
    i = 0;
    element = ll.first;
    while (element != NULL)
    {
        EXPECT_NE(element, element5);
        element = element->next;
        ++i;
    }
    EXPECT_EQ(i, 9);

    // remove the last element
    linked_list_element_t* lastptr = ll.last;
    linked_list_remove(&ll, ll.last);
    release(&alloc_opts, lastptr);
    EXPECT_EQ(ll.elements, 8UL);
    EXPECT_EQ(*(int*)ll.last->data, 8);

    // remove the first element
    linked_list_element_t* firstptr = ll.first;
    linked_list_remove(&ll, ll.first);
    release(&alloc_opts, firstptr);
    EXPECT_EQ(ll.elements, 7UL);
    EXPECT_EQ(*(int*)ll.first->data, 1);

    // remove all remaining elements
    for (i = 0; i < 7; i++)
    {
        linked_list_element_t* curr = ll.first;
        linked_list_remove(&ll, ll.first);
        release(&alloc_opts, curr);
    }
    EXPECT_EQ(ll.elements, 0UL);
    EXPECT_EQ(ll.first, nullptr);
    EXPECT_EQ(ll.last, nullptr);

    //dispose of our list
    dispose((disposable_t*)&ll);
}

void build_linked_list(linked_list_t* ll, int* data, int n)
{
    ASSERT_NE(ll, nullptr);
    ASSERT_NE(data, nullptr);
    ASSERT_NE(n, 0);

    for (int i = 0; i < n; i++)
    {
        ASSERT_EQ(VPR_STATUS_SUCCESS, linked_list_insert_end(ll, &data[i]));
    }
}
