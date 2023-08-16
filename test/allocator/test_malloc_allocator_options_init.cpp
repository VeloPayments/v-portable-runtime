/**
 * \file test_malloc_allocator_options_init.cpp
 *
 * Unit tests for malloc_allocator_options_init.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>
#include <minunit/minunit.h>

TEST_SUITE(malloc_allocator_options_init_test);

/**
 * There's not much we can test here.  This function should be manually
 * reviewed.  We can at least test that some fields are set, and that the
 * context is null.
 */
TEST(initTest)
{
    allocator_options_t options;

    malloc_allocator_options_init(&options);

    TEST_EXPECT(nullptr != (void*)options.allocator_allocate);
    TEST_EXPECT(nullptr != (void*)options.allocator_release);
    TEST_EXPECT(nullptr != (void*)options.allocator_reallocate);
    TEST_EXPECT(nullptr != (void*)options.allocator_control);
    TEST_EXPECT(0 == options.context);

    dispose(allocator_options_disposable_handle(&options));
}

/**
 * There are no valid control knobs for malloc allocator.
 */
TEST(allocator_control)
{
    allocator_options_t options;

    malloc_allocator_options_init(&options);

    TEST_EXPECT(
        VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY
            == allocator_control(&options, 1, nullptr));

    dispose(allocator_options_disposable_handle(&options));
}
