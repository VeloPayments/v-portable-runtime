/**
 * \file test_linked_list_remove.cpp
 *
 * Unit tests for linked_list_remove
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

void build_linked_list(linked_list_t* ll, int* data, int n);

class ll_remove_test {
public:
    void setUp()
    {
        malloc_allocator_options_init(&alloc_opts);
        linked_list_options_init_status =
            linked_list_options_init(&options, &alloc_opts,
                false, sizeof(int), false);
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

TEST_SUITE(ll_remove_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    ll_remove_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(options_init)
    TEST_ASSERT(VPR_STATUS_SUCCESS == fixture.linked_list_options_init_status);
END_TEST_F()

BEGIN_TEST_F(basic_test)
    linked_list_t ll;

    TEST_ASSERT(linked_list_init(&fixture.options, &ll) == 0);

    int data_elements[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    build_linked_list(&ll, data_elements, 10);

    // there should be 10 elements, all in order
    TEST_EXPECT(ll.elements == 10UL);
    linked_list_element_t* element = ll.first;
    linked_list_element_t* element5 = NULL;
    int i = 0;
    while (element != NULL)
    {

        TEST_EXPECT(*(int*)element->data == data_elements[i]);
        if (i == 5)
        {
            element5 = element;
        }
        element = element->next;
        ++i;
    }
    TEST_EXPECT(i == 10);
    TEST_EXPECT(element5 != nullptr);

    // now remove the fifth element
    linked_list_remove(&ll, element5);
    release(&fixture.alloc_opts, element5);
    TEST_EXPECT(ll.elements == 9UL);

    // traverse the list again, ensuring the removed element isn't present
    i = 0;
    element = ll.first;
    while (element != NULL)
    {
        TEST_EXPECT(element != element5);
        element = element->next;
        ++i;
    }
    TEST_EXPECT(i == 9);

    // remove the last element
    linked_list_element_t* lastptr = ll.last;
    linked_list_remove(&ll, ll.last);
    release(&fixture.alloc_opts, lastptr);
    TEST_EXPECT(ll.elements == 8UL);
    TEST_EXPECT(*(int*)ll.last->data == 8);

    // remove the first element
    linked_list_element_t* firstptr = ll.first;
    linked_list_remove(&ll, ll.first);
    release(&fixture.alloc_opts, firstptr);
    TEST_EXPECT(ll.elements == 7UL);
    TEST_EXPECT(*(int*)ll.first->data == 1);

    // remove all remaining elements
    for (i = 0; i < 7; i++)
    {
        linked_list_element_t* curr = ll.first;
        linked_list_remove(&ll, ll.first);
        release(&fixture.alloc_opts, curr);
    }
    TEST_EXPECT(ll.elements == 0UL);
    TEST_EXPECT(ll.first == nullptr);
    TEST_EXPECT(ll.last == nullptr);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
END_TEST_F()

void build_linked_list(linked_list_t* ll, int* data, int n)
{
    assert(ll != nullptr);
    assert(data != nullptr);
    assert(n != 0);

    for (int i = 0; i < n; i++)
    {
        assert(VPR_STATUS_SUCCESS == linked_list_insert_end(ll, &data[i]));
    }
}
