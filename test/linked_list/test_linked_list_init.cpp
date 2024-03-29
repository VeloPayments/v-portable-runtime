/**
 * \file test_linked_list_init.cpp
 *
 * Unit tests for linked_list_init.
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

class ll_init_test {
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

TEST_SUITE(ll_init_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    ll_init_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(options_init)
    TEST_ASSERT(VPR_STATUS_SUCCESS == fixture.linked_list_options_init_status);
END_TEST_F()

BEGIN_TEST_F(basic_test)
    linked_list ll;

    TEST_ASSERT(linked_list_init(&fixture.options, &ll) == 0);

    // the number of elements should be initialized to 0, with
    // the first and last pointers pointing to NULL
    TEST_EXPECT(ll.elements == 0UL);
    TEST_EXPECT(ll.first == nullptr);
    TEST_EXPECT(ll.last == nullptr);

    //dispose of our list
    dispose(linked_list_disposable_handle(&ll));
END_TEST_F()
