/**
 * \file test_linked_list_insert_before.cpp
 *
 * Unit tests for linked_list_insert_before
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

class ll_insert_before_test : public ::testing::Test {
protected:
    void SetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        linked_list_options_init(&options, &alloc_opts,
            copy_on_insert, sizeof(long), false);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    linked_list_options_t options;
};

TEST_F(ll_insert_before_test, basic_test)
{
    SetUp(true);

    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    long data = 2023988L;

    ASSERT_EQ(linked_list_insert_end(&ll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    EXPECT_EQ(ll.elements, 1UL);
    EXPECT_EQ(ll.first, ll.last);

    // the pointer on the initial element should be set to NULL
    EXPECT_EQ(ll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(long*)(ll.first->data), 2023988L);

    // insert an element before the initial one
    long data2 = 205L;
    ASSERT_EQ(linked_list_insert_before(&ll, ll.first, &data2), 0);

    // the number of elements should be 2, with the first element being the
    // second data item and the last element being the first data item
    EXPECT_EQ(ll.elements, 2UL);
    EXPECT_EQ(*(long*)(ll.first->data), data2);
    EXPECT_EQ(*(long*)(ll.last->data), data);

    // test the links on each element
    EXPECT_EQ(ll.first->next, ll.last);
    EXPECT_EQ(ll.last->next, nullptr);

    // add one more element, before the initial element
    // now the order should be data3 -> data2 -> data
    long data3 = -992L;
    ASSERT_EQ(linked_list_insert_before(&ll, ll.first, &data3), 0);
    EXPECT_EQ(ll.elements, 3UL);
    EXPECT_EQ(*(long*)(ll.first->data), data3);
    EXPECT_EQ(*(long*)(ll.first->next->data), data2);
    EXPECT_EQ(*(long*)(ll.first->next->next->data), data);
    EXPECT_EQ(*(long*)(ll.last->data), data);

    // test some pointers
    EXPECT_EQ(ll.first->next->next, ll.last);
    EXPECT_EQ(ll.last->next, nullptr);


    //dispose of our list
    dispose((disposable_t*)&ll);
}

TEST_F(ll_insert_before_test, with_copy_on_insert)
{
    SetUp(true);

    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    long data = 356;
    ASSERT_EQ(linked_list_insert_end(&ll, &data), 0);

    // insert something after the initial element
    long data2 = 205;
    ASSERT_EQ(linked_list_insert_after(&ll, ll.first, &data2), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(long*)(ll.first->data), data - 1);

    data2 += 10;
    EXPECT_EQ(*(long*)(ll.last->data), data2 - 10);


    //dispose of our list
    dispose((disposable_t*)&ll);
}

TEST_F(ll_insert_before_test, without_copy_on_insert)
{
    SetUp(false);

    linked_list_t ll;

    ASSERT_EQ(linked_list_init(&options, &ll), 0);

    // insert some data
    long data = 356;
    ASSERT_EQ(linked_list_insert_end(&ll, &data), 0);

    // insert something after the initial element
    long data2 = 205;
    ASSERT_EQ(linked_list_insert_after(&ll, ll.first, &data2), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(long*)(ll.first->data), data);

    data2 += 10;
    EXPECT_EQ(*(long*)(ll.last->data), data2);


    //dispose of our list
    dispose((disposable_t*)&ll);
}
