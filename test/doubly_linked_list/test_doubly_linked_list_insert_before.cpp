/**
 * \file test_doubly_linked_list_insert_before.cpp
 *
 * Unit tests for doubly_linked_list_insert_before.
 *
 * \copyright 2019-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

/* DISABLED GTEST */
#if 0
class dll_insert_before_test : public ::testing::Test {
protected:
    void LocalSetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init_status =
            doubly_linked_list_options_init(&options, &alloc_opts,
                copy_on_insert, sizeof(long), false);
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

TEST_F(dll_insert_before_test, options_init)
{
    LocalSetUp(true);
    ASSERT_EQ(VPR_STATUS_SUCCESS, doubly_linked_list_options_init_status);
}

TEST_F(dll_insert_before_test, basic_test)
{
    LocalSetUp(true);

    doubly_linked_list_t dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    long data = 2023988L;

    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    EXPECT_EQ(dll.elements, 1UL);
    EXPECT_EQ(dll.first, dll.last);

    // the pointers on the initial element should be set to NULL
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next, nullptr);

    // and the data should be as expected
    EXPECT_EQ(*(long*)(dll.first->data), 2023988L);

    // insert an element before the initial one
    long data2 = 205L;
    ASSERT_EQ(doubly_linked_list_insert_before(&dll, dll.first, &data2), 0);

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

    // add one more element, before the initial element
    // now the order should be data3 -> data2 -> data
    long data3 = -992L;
    ASSERT_EQ(doubly_linked_list_insert_before(&dll, dll.first, &data3), 0);
    EXPECT_EQ(dll.elements, 3UL);
    EXPECT_EQ(*(long*)(dll.first->data), data3);
    EXPECT_EQ(*(long*)(dll.first->next->data), data2);
    EXPECT_EQ(*(long*)(dll.first->next->next->data), data);

    // now in reverse
    EXPECT_EQ(*(long*)(dll.last->data), data);
    EXPECT_EQ(*(long*)(dll.last->prev->data), data2);
    EXPECT_EQ(*(long*)(dll.last->prev->prev->data), data3);

    // test some pointers
    EXPECT_EQ(dll.first->prev, nullptr);
    EXPECT_EQ(dll.first->next->next, dll.last);
    EXPECT_EQ(dll.first->next, dll.last->prev);
    EXPECT_EQ(dll.last->prev->prev, dll.first);
    EXPECT_EQ(dll.last->next, nullptr);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
}

TEST_F(dll_insert_before_test, with_copy_on_insert)
{
    LocalSetUp(true);

    doubly_linked_list_t dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    long data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // insert something after the initial element
    long data2 = 205;
    ASSERT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);

    // test that the data was copied
    data++;
    EXPECT_EQ(*(long*)(dll.first->data), data - 1);

    data2 += 10;
    EXPECT_EQ(*(long*)(dll.last->data), data2 - 10);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
}

TEST_F(dll_insert_before_test, without_copy_on_insert)
{
    LocalSetUp(false);

    doubly_linked_list_t dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // insert some data
    long data = 356;
    ASSERT_EQ(doubly_linked_list_insert_beginning(&dll, &data), 0);

    // insert something after the initial element
    long data2 = 205;
    ASSERT_EQ(doubly_linked_list_insert_after(&dll, dll.first, &data2), 0);

    // test that the data was NOT copied
    data++;
    EXPECT_EQ(*(long*)(dll.first->data), data);

    data2 += 10;
    EXPECT_EQ(*(long*)(dll.last->data), data2);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
}
#endif
