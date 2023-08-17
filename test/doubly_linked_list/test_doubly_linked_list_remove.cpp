/**
 * \file test_doubly_linked_list_remove.cpp
 *
 * Unit tests for doubly_linked_list_remove
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

static int build_doubly_linked_list(
    doubly_linked_list_t* dll, int* data, int n);


class dll_remove_test {
public:
    void setUp()
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init_success =
            doubly_linked_list_options_init(&options, &alloc_opts,
                false, sizeof(int), false);
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

TEST_SUITE(dll_remove_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dll_remove_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(options_init)
    TEST_ASSERT(
        VPR_STATUS_SUCCESS == fixture.doubly_linked_list_options_init_success);
END_TEST_F()

BEGIN_TEST_F(basic_test)
    doubly_linked_list_t dll;

    TEST_ASSERT(doubly_linked_list_init(&fixture.options, &dll) == 0);

    int data_elements[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    TEST_ASSERT(0 == build_doubly_linked_list(&dll, data_elements, 10));

    // there should be 10 elements, all in order
    TEST_EXPECT(dll.elements == 10UL);
    doubly_linked_list_element_t* element = dll.first;
    doubly_linked_list_element_t* element5 = NULL;
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
    doubly_linked_list_remove(&dll, element5);
    release(&fixture.alloc_opts, element5);
    TEST_EXPECT(dll.elements == 9UL);

    // traverse the list again, ensuring the removed element isn't present
    i = 0;
    element = dll.first;
    while (element != NULL)
    {
        TEST_EXPECT(element != element5);
        element = element->next;
        ++i;
    }
    TEST_EXPECT(i == 9);

    // remove the last element
    doubly_linked_list_element_t* lastptr = dll.last;
    doubly_linked_list_remove(&dll, dll.last);
    release(&fixture.alloc_opts, lastptr);
    TEST_EXPECT(dll.elements == 8UL);
    TEST_EXPECT(*(int*)dll.last->data == 8);

    // remove the first element
    doubly_linked_list_element_t* firstptr = dll.first;
    doubly_linked_list_remove(&dll, dll.first);
    release(&fixture.alloc_opts, firstptr);
    TEST_EXPECT(dll.elements == 7UL);
    TEST_EXPECT(*(int*)dll.first->data == 1);

    // remove all remaining elements
    for (i = 0; i < 7; i++)
    {
        doubly_linked_list_element_t* curr = dll.first;
        doubly_linked_list_remove(&dll, dll.first);
        release(&fixture.alloc_opts, curr);
    }
    TEST_EXPECT(dll.elements == 0UL);
    TEST_EXPECT(dll.first == nullptr);
    TEST_EXPECT(dll.last == nullptr);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
END_TEST_F()

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
