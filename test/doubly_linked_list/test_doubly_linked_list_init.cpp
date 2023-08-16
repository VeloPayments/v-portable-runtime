/**
 * \file test_doubly_linked_list_init.cpp
 *
 * Unit tests for doubly_linked_list_init.
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

class dll_init_test {
public:
    void setUp()
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init_status =
            doubly_linked_list_options_init(&options, &alloc_opts,
                false, sizeof(int), false);
    }

    void tearDown()
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

TEST_SUITE(dll_init_dest);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dll_init_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(basic_test)
    doubly_linked_list dll;

    // verify that options init succeeded
    TEST_ASSERT(
        VPR_STATUS_SUCCESS == fixture.doubly_linked_list_options_init_status);

    TEST_ASSERT(doubly_linked_list_init(&fixture.options, &dll) == 0);

    // the number of elements should be initialized to 0, with
    // the first and last pointers pointing to NULL
    TEST_EXPECT(dll.elements == 0UL);
    TEST_EXPECT(dll.first == nullptr);
    TEST_EXPECT(dll.last == nullptr);

    //dispose of our list
    dispose(doubly_linked_list_disposable_handle(&dll));
END_TEST_F()
