/**
 * \file test_malloc_allocator_options_init.cpp
 *
 * Unit tests for malloc_allocator_options_init.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>

/**
 * There's not much we can test here.  This function should be manually
 * reviewed.  We can at least test that some fields are set, and that the
 * context is null.
 */
TEST(malloc_allocator_options_init_test, initTest)
{
    allocator_options_t options;

    malloc_allocator_options_init(&options);

    EXPECT_NE(nullptr, (void*)options.allocator_allocate);
    EXPECT_NE(nullptr, (void*)options.allocator_release);
    EXPECT_NE(nullptr, (void*)options.allocator_reallocate);
    EXPECT_EQ(0, options.context);

    dispose((disposable_t*)&options);
}
