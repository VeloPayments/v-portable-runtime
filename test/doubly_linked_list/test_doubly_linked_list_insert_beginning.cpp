/**
 * \file test_doubly_linked_list_insert_beginning.cpp
 *
 * Unit tests for doubly_linked_list_insert_beginning
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

class dll_insert_beginning_test {
public:
    void localSetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init_success =
            doubly_linked_list_options_init(&options, &alloc_opts,
                copy_on_insert, sizeof(int), false);
    }

    void tearDown()
    {
        if (VPR_STATUS_SUCCESS == doubly_linked_list_options_init_success)
        {
            dispose(doubly_linked_list_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int doubly_linked_list_options_init_success;
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_SUITE(bloom_filter_hash_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dll_insert_beginning_test fixture; \

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(options_init)
    fixture.localSetUp(true);
    TEST_ASSERT(
        VPR_STATUS_SUCCESS == fixture.doubly_linked_list_options_init_success);
END_TEST_F()

BEGIN_TEST_F(basic_test)
    fixture.localSetUp(true);

    doubly_linked_list_t dll;

    TEST_ASSERT(doubly_linked_list_init(&fixture.options, &dll) == 0);

    int data = 356;

    TEST_ASSERT(doubly_linked_list_insert_beginning(&dll, &data) == 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    TEST_EXPECT(dll.elements == 1UL);
    TEST_EXPECT(dll.first == dll.last);

    // the pointers on the initial element should be set to NULL
    TEST_EXPECT(dll.first->prev == nullptr);
    TEST_EXPECT(dll.first->next == nullptr);

    // and the data should be as expected
    TEST_EXPECT(*(int*)(dll.first->data) == data);

    // insert something new at the beginning
    int data2 = 205;
    TEST_ASSERT(doubly_linked_list_insert_beginning(&dll, &data2) == 0);

    // the number of elements should be 2, with the first element being the
    // second data item and the last element being the first data item
    TEST_EXPECT(dll.elements == 2UL);
    TEST_EXPECT(*(int*)(dll.first->data) == data2);
    TEST_EXPECT(*(int*)(dll.last->data) == data);

    // test the links on each element
    TEST_EXPECT(dll.first->prev == nullptr);
    TEST_EXPECT(dll.first->next == dll.last);
    TEST_EXPECT(dll.last->prev == dll.first);
    TEST_EXPECT(dll.last->next == nullptr);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
END_TEST_F()

BEGIN_TEST_F(with_copy_on_insert)
    fixture.localSetUp(true);

    doubly_linked_list_t dll;

    TEST_ASSERT(doubly_linked_list_init(&fixture.options, &dll) == 0);

    // insert some data
    int data = 356;
    TEST_ASSERT(doubly_linked_list_insert_beginning(&dll, &data) == 0);

    // test that the data was copied
    data++;
    TEST_EXPECT(*(int*)(dll.first->data) == data - 1);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
END_TEST_F()

BEGIN_TEST_F(without_copy_on_insert)
    fixture.localSetUp(false);

    doubly_linked_list_t dll;

    TEST_ASSERT(doubly_linked_list_init(&fixture.options, &dll) == 0);

    // insert some data
    int data = 356;
    TEST_ASSERT(doubly_linked_list_insert_beginning(&dll, &data) == 0);

    // test that the data was NOT copied
    data++;
    TEST_EXPECT(*(int*)(dll.first->data) == data);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
END_TEST_F()
