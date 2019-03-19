/**
 * \file test_doubly_linked_list_init.cpp
 *
 * Unit tests for doubly_linked_list_init.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

class dll_init_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        doubly_linked_list_options_init(&options, &alloc_opts,
            false, sizeof(int), false);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
};

TEST_F(dll_init_test, basic_test)
{
    doubly_linked_list dll;

    ASSERT_EQ(doubly_linked_list_init(&options, &dll), 0);

    // the number of elements should be initialized to 0, with
    // the first and last pointers pointing to NULL
    EXPECT_EQ(dll.elements, 0UL);
    EXPECT_EQ(dll.first, nullptr);
    EXPECT_EQ(dll.last, nullptr);

    //dispose of our list
    dispose((disposable_t*)&dll);
}
