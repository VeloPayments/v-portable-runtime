/**
 * \file test_linked_list_insert_end.cpp
 *
 * Unit tests for linked_list_insert_end
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

class ll_insert_end_test {
public:
    void localSetUp(bool copy_on_insert)
    {
        malloc_allocator_options_init(&alloc_opts);
        linked_list_options_init_status =
            linked_list_options_init(&options, &alloc_opts,
                copy_on_insert, sizeof(int), false);
    }

    void tearDown()
    {
        if (VPR_STATUS_SUCCESS == linked_list_options_init_status)
        {
            dispose(linked_list_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int linked_list_options_init_status;
    allocator_options_t alloc_opts;
    linked_list_options_t options;
};

TEST_SUITE(ll_insert_end_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    ll_insert_end_test fixture;

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(options_init)
    fixture.localSetUp(true);
    TEST_ASSERT(VPR_STATUS_SUCCESS == fixture.linked_list_options_init_status);
END_TEST_F()

BEGIN_TEST_F(basic_test)
    fixture.localSetUp(true);

    linked_list ll;

    TEST_ASSERT(linked_list_init(&fixture.options, &ll) == 0);

    int data = 356;
    TEST_ASSERT(linked_list_insert_end(&ll, &data) == 0);

    // the number of elements should be 1, with
    // the first and last pointers pointing this element
    TEST_EXPECT(ll.elements == 1UL);
    TEST_EXPECT(ll.first == ll.last);

    // the pointer on the initial element should be set to NULL
    TEST_EXPECT(ll.first->next == nullptr);

    // and the data should be as expected
    TEST_EXPECT(*(int*)(ll.first->data) == data);
    TEST_EXPECT(*(int*)(ll.last->data) == data);

    // insert something new at the end
    int data2 = 205;
    TEST_ASSERT(linked_list_insert_end(&ll, &data2) == 0);

    // the number of elements should be 2, with the first element being the
    // second data item and the last element being the first data item
    TEST_EXPECT(ll.elements == 2UL);
    TEST_EXPECT(*(int*)(ll.first->data) == data);
    TEST_EXPECT(*(int*)(ll.last->data) == data2);

    // test the links on each element
    TEST_EXPECT(ll.first->next == ll.last);
    TEST_EXPECT(ll.last->next == nullptr);

    // insert a third element on the end
    int data3 = 301;
    TEST_ASSERT(linked_list_insert_end(&ll, &data3) == 0);

    // should be three elements: data -> data2 -> data3
    TEST_EXPECT(ll.elements == 3UL);
    TEST_EXPECT(*(int*)(ll.first->data) == data);
    TEST_EXPECT(*(int*)(ll.first->next->data) == data2);
    TEST_EXPECT(*(int*)(ll.first->next->next->data) == data3);

    // test some pointers
    TEST_EXPECT(ll.first->next->next == ll.last);
    TEST_EXPECT(ll.last->next == nullptr);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
END_TEST_F()

BEGIN_TEST_F(with_copy_on_insert)
    fixture.localSetUp(true);

    linked_list ll;

    TEST_ASSERT(linked_list_init(&fixture.options, &ll) == 0);

    // insert some data
    int data = 356;
    TEST_ASSERT(linked_list_insert_end(&ll, &data) == 0);

    // insert something new at the end
    int data2 = 205;
    TEST_ASSERT(linked_list_insert_end(&ll, &data2) == 0);

    // test that the data was copied
    data++;
    TEST_EXPECT(*(int*)(ll.first->data) == data - 1);

    data2 += 10;
    TEST_EXPECT(*(int*)(ll.last->data) == data2 - 10);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
END_TEST_F()

BEGIN_TEST_F(without_copy_on_insert)
    fixture.localSetUp(false);

    linked_list ll;

    TEST_ASSERT(linked_list_init(&fixture.options, &ll) == 0);

    // insert some data
    int data = 356;
    TEST_ASSERT(linked_list_insert_end(&ll, &data) == 0);

    // insert something new at the end
    int data2 = 205;
    TEST_ASSERT(linked_list_insert_end(&ll, &data2) == 0);

    // test that the data was NOT copied
    data++;
    TEST_EXPECT(*(int*)(ll.first->data) == data);

    data2 += 10;
    TEST_EXPECT(*(int*)(ll.last->data) == data2);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
END_TEST_F()
