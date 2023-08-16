/**
 * \file test_bump_allocator_options_init.cpp
 *
 * Unit tests for bump_allocator_options_init.
 *
 * \copyright 2018-2020 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator.h>
#include <vpr/allocator/bump_allocator.h>

/* DISABLED GTEST */
#if 0

/**
 * If we initialize the bump allocator with a tiny buffer and then make a larger
 * allocation request, allocation fails.
 */
TEST(bump_allocator_options_init, small_buffer_large_allocation)
{
    allocator_options_t options;
    uint8_t buffer[bump_allocator_minimum_buffer_size + 12];

    /* we can make a bump allocator with a small arena. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        bump_allocator_options_init(&options, buffer, sizeof(buffer)));

    /* allocation should fail because the arena is too small. */
    ASSERT_EQ(nullptr, allocate(&options, 100));

    /* clean up. */
    dispose(allocator_options_disposable_handle(&options));
}

/**
 * Verify that a valid allocation is within the range of the backing buffer.
 */
TEST(bump_allocator_options_init, simple_allocation)
{
    allocator_options_t options;
    uint8_t buffer[bump_allocator_minimum_buffer_size + 100];

    ASSERT_EQ(VPR_STATUS_SUCCESS,
        bump_allocator_options_init(&options, buffer, sizeof(buffer)));

    /* allocation should succeed. */
    uint8_t* ptr = (uint8_t*)allocate(&options, 10);
    ASSERT_NE(nullptr, ptr);

    /* verify that the pointer exists between the bounds of the buffer and the
     * buffer size.
     */
    EXPECT_GT((size_t)ptr, (size_t)buffer);
    EXPECT_LT((size_t)ptr + 10, (size_t)(buffer + sizeof(buffer)));

    /* verify that this buffer is aligned on a 128-bit boundary. */
    EXPECT_EQ(0UL, ((size_t)ptr) % 16);

    /* clean up. */
    dispose(allocator_options_disposable_handle(&options));
}

/**
 * Verify that resetting the bump allocator works.
 */
TEST(bump_allocator_options_init, bump_reset)
{
    allocator_options_t options;
    uint8_t buffer[bump_allocator_minimum_buffer_size + 100];

    ASSERT_EQ(VPR_STATUS_SUCCESS,
        bump_allocator_options_init(&options, buffer, sizeof(buffer)));

    /* allocation should succeed. */
    uint8_t* ptr_x1 = (uint8_t*)allocate(&options, 10);
    uint8_t* ptr_y1 = (uint8_t*)allocate(&options, 10);
    EXPECT_NE(nullptr, ptr_x1);
    EXPECT_NE(nullptr, ptr_y1);

    /* resetting the bump allocator should succeed. */
    ASSERT_EQ(VPR_STATUS_SUCCESS,
        allocator_control(&options,
            ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET,
            nullptr));

    /* allocation should succeed. */
    uint8_t* ptr_x2 = (uint8_t*)allocate(&options, 10);
    uint8_t* ptr_y2 = (uint8_t*)allocate(&options, 10);
    EXPECT_NE(nullptr, ptr_x2);
    EXPECT_NE(nullptr, ptr_y2);

    /* since the bump allocator was reset, the pointers should line up. */
    EXPECT_EQ(ptr_x1, ptr_x2);
    EXPECT_EQ(ptr_y1, ptr_y2);

    /* clean up. */
    dispose(allocator_options_disposable_handle(&options));
}
#endif
