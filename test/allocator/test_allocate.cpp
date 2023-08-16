/**
 * \file test_allocate.cpp
 *
 * Unit tests for allocator.allocate.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator.h>

#include "mock_allocator.h"

TEST_SUITE(allocator_allocate);

/**
 * Test that allocate calls the allocator_allocate method in the options
 * structure.
 */
TEST(basicTest)
{
    const size_t SIZE = 42;

    allocator_options_t options;

    //initialize options with our mock
    mock_allocator_options_init(&options, false);

    //call to mock always returns a null pointer
    TEST_EXPECT(nullptr == allocate(&options, SIZE));

    //the allocator_allocate mock should have been called with our arguments
    TEST_EXPECT(mock_allocator_allocate_called(&options, SIZE));

    //dispose options now that we're done
    dispose(allocator_options_disposable_handle(&options));
}
