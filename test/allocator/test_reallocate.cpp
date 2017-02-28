/**
 * \file test_reallocate.cpp
 *
 * Unit tests for allocator.reallocate.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator.h>

#include "mock_allocator.h"

/**
 * Test that reallocate calls the allocator_reallocate method when it is
 * defined.
 */
TEST(allocator_reallocate, reallocateOptionTest)
{
    void* MEM = (void*)1234;
    const size_t OLD_SIZE = 99;
    const size_t NEW_SIZE = 17;

    allocator_options_t options;

    //initialize options with our mock (reallocate enabled)
    mock_allocator_options_init(&options, true);

    //calling the mock returns null
    EXPECT_EQ(nullptr, reallocate(&options, MEM, OLD_SIZE, NEW_SIZE));

    //the allocator_reallocate mock should have been called with our arguments
    EXPECT_TRUE(
        mock_allocator_reallocate_called(&options, MEM, OLD_SIZE, NEW_SIZE));

    //dispose options now that we're done
    dispose((disposable_t*)&options);
}

/**
 * Test that we fall back to using allocate / release when allocate_reallocate
 * is not defined.
 */
TEST(allocator_reallocate, noReallocateTest)
{
    char old_buffer[200];
    char new_buffer[400];
    void* MEM = (void*)old_buffer;
    void* ALLOC_RETVAL = (void*)new_buffer;
    const size_t OLD_SIZE = sizeof(old_buffer);
    const size_t NEW_SIZE = sizeof(new_buffer);

    allocator_options_t options;

    //set up buffers for test
    memset(old_buffer, 55, sizeof(old_buffer));
    memset(new_buffer, 88, sizeof(new_buffer));

    //initialize options with our mock (reallocate disabled)
    mock_allocator_options_init(&options, false);

    //happy path: allocate succeeds
    mock_allocator_allocate_retval(&options, ALLOC_RETVAL);

    //calling our method returns the ALLOC_RETVAL value.
    EXPECT_EQ(ALLOC_RETVAL, reallocate(&options, MEM, OLD_SIZE, NEW_SIZE));

    //the allocator_reallocate mock should NOT have been called.
    EXPECT_FALSE(
        mock_allocator_reallocate_called(&options));

    //the allocator_allocate mock should have been called
    EXPECT_TRUE(
        mock_allocator_allocate_called(&options, NEW_SIZE));

    //the allocator_release mock should have been called
    EXPECT_TRUE(
        mock_allocator_release_called(&options, MEM));

    //OLD_SIZE bytes from the old buffer should have been copied to the
    //new buffers
    EXPECT_EQ(0, memcmp(old_buffer, new_buffer, OLD_SIZE));

    //dispose options now that we're done
    dispose((disposable_t*)&options);
}

/**
 * Test that we truncate when NEW_SIZE < OLD_SIZE and allocator_reallocate is
 * disabled.
 */
TEST(allocator_reallocate, noReallocateTruncate)
{
    char old_buffer[200];
    char new_buffer[100];
    void* MEM = (void*)old_buffer;
    void* ALLOC_RETVAL = (void*)new_buffer;
    const size_t OLD_SIZE = sizeof(old_buffer);
    const size_t NEW_SIZE = sizeof(new_buffer);

    allocator_options_t options;

    //set up buffers for test
    memset(old_buffer, 55, sizeof(old_buffer));
    memset(new_buffer, 88, sizeof(new_buffer));

    //initialize options with our mock (reallocate disabled)
    mock_allocator_options_init(&options, false);

    //happy path: allocate succeeds
    mock_allocator_allocate_retval(&options, ALLOC_RETVAL);

    //calling our method returns the ALLOC_RETVAL value.
    EXPECT_EQ(ALLOC_RETVAL, reallocate(&options, MEM, OLD_SIZE, NEW_SIZE));

    //the allocator_reallocate mock should NOT have been called.
    EXPECT_FALSE(
        mock_allocator_reallocate_called(&options));

    //the allocator_allocate mock should have been called
    EXPECT_TRUE(
        mock_allocator_allocate_called(&options, NEW_SIZE));

    //the allocator_release mock should have been called
    EXPECT_TRUE(
        mock_allocator_release_called(&options, MEM));

    //NEW_SIZE bytes from the old buffer should have been copied to the
    //new buffers
    EXPECT_EQ(0, memcmp(old_buffer, new_buffer, NEW_SIZE));

    //dispose options now that we're done
    dispose((disposable_t*)&options);
}
