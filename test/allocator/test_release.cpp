/**
 * \file test_release.cpp
 *
 * Unit tests for allocator.release.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator.h>
#include <minunit/minunit.h>

#include "mock_allocator.h"

TEST_SUITE(allocator_release);

/**
 * Test that release calls the allocator_release method in the options
 * structure.
 */
TEST(basicTest)
{
    void* MEM = (void*)5678;

    allocator_options_t options;

    //initialize options with our mock
    mock_allocator_options_init(&options, false);

    //call the release method
    release(&options, MEM);

    //the allocator_release mock should have been called with our arguments
    TEST_EXPECT(mock_allocator_release_called(&options, MEM));

    //dispose options now that we're done
    dispose(allocator_options_disposable_handle(&options));
}
