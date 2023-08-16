/**
 * \file test_malloc_allocator_options_init.cpp
 *
 * Unit tests for malloc_allocator_options_init.
 *
 * \copyright 2017-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>

/* DISABLED GTEST */
#if 0

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
    EXPECT_NE(nullptr, (void*)options.allocator_control);
    EXPECT_EQ(0, options.context);

    dispose(allocator_options_disposable_handle(&options));
}

/**
 * There are no valid control knobs for malloc allocator.
 */
TEST(malloc_allocator_options_init_test, allocator_control)
{
    allocator_options_t options;

    malloc_allocator_options_init(&options);

    EXPECT_EQ(VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY,
        allocator_control(&options, 1, nullptr));

    dispose(allocator_options_disposable_handle(&options));
}
#endif
