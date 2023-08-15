/**
 * \file test_allocate.cpp
 *
 * Unit tests for allocator.allocate.
 *
 * \copyright 2017-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator.h>

/* DISABLED GTEST */
#if 0

#include "mock_allocator.h"

/**
 * Test that allocate calls the allocator_allocate method in the options
 * structure.
 */
TEST(allocator_allocate, basicTest)
{
    const size_t SIZE = 42;

    allocator_options_t options;

    //initialize options with our mock
    mock_allocator_options_init(&options, false);

    //call to mock always returns a null pointer
    EXPECT_EQ(nullptr, allocate(&options, SIZE));

    //the allocator_allocate mock should have been called with our arguments
    EXPECT_TRUE(mock_allocator_allocate_called(&options, SIZE));

    //dispose options now that we're done
    dispose(allocator_options_disposable_handle(&options));
}
#endif
