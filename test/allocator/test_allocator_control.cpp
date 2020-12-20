/**
 * \file test_allocator_control.cpp
 *
 * Unit tests for allocator.allocator_control.
 *
 * \copyright 2018-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator.h>

#include "mock_allocator.h"

/**
 * Test that allocator_control calls the allocator_control method.
 */
TEST(allocator_control, happy_path)
{
    const int KEY = 0x12340000;
    void* VALUE = (void*)0x00100000;

    allocator_options_t options;

    //initialize options with our mock (reallocate disabled)
    mock_allocator_options_init(&options, false);

    //calling the mock returns success
    EXPECT_EQ(VPR_STATUS_SUCCESS, allocator_control(&options, KEY, VALUE));

    //the allocator_reallocate mock should have been called with our arguments
    EXPECT_TRUE(
        mock_allocator_control_called(&options, KEY, VALUE));

    //dispose options now that we're done
    dispose(allocator_options_disposable_handle(&options));
}
