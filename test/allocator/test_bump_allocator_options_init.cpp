/**
 * \file test_bump_allocator_options_init.cpp
 *
 * Unit tests for bump_allocator_options_init.
 *
 * \copyright 2018-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <vpr/allocator.h>
#include <vpr/allocator/bump_allocator.h>
#include <minunit/minunit.h>

TEST_SUITE(bump_allocator_options_init);

/**
 * If we initialize the bump allocator with a tiny buffer and then make a larger
 * allocation request, allocation fails.
 */
TEST(small_buffer_large_allocation)
{
    allocator_options_t options;
    uint8_t buffer[bump_allocator_minimum_buffer_size + 12];

    /* we can make a bump allocator with a small arena. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == bump_allocator_options_init(&options, buffer, sizeof(buffer)));

    /* allocation should fail because the arena is too small. */
    TEST_ASSERT(nullptr == allocate(&options, 100));

    /* clean up. */
    dispose(allocator_options_disposable_handle(&options));
}

/**
 * Verify that a valid allocation is within the range of the backing buffer.
 */
TEST(simple_allocation)
{
    allocator_options_t options;
    uint8_t buffer[bump_allocator_minimum_buffer_size + 100];

    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == bump_allocator_options_init(&options, buffer, sizeof(buffer)));

    /* allocation should succeed. */
    uint8_t* ptr = (uint8_t*)allocate(&options, 10);
    TEST_ASSERT(nullptr != ptr);

    /* verify that the pointer exists between the bounds of the buffer and the
     * buffer size.
     */
    TEST_EXPECT((size_t)ptr > (size_t)buffer);
    TEST_EXPECT((size_t)ptr + 10 < (size_t)(buffer + sizeof(buffer)));

    /* verify that this buffer is aligned on a 128-bit boundary. */
    TEST_EXPECT(0UL == ((size_t)ptr) % 16);

    /* clean up. */
    dispose(allocator_options_disposable_handle(&options));
}

/**
 * Verify that resetting the bump allocator works.
 */
TEST(bump_reset)
{
    allocator_options_t options;
    uint8_t buffer[bump_allocator_minimum_buffer_size + 100];

    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == bump_allocator_options_init(&options, buffer, sizeof(buffer)));

    /* allocation should succeed. */
    uint8_t* ptr_x1 = (uint8_t*)allocate(&options, 10);
    uint8_t* ptr_y1 = (uint8_t*)allocate(&options, 10);
    TEST_EXPECT(nullptr != ptr_x1);
    TEST_EXPECT(nullptr != ptr_y1);

    /* resetting the bump allocator should succeed. */
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == allocator_control(&options,
                    ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET, nullptr));

    /* allocation should succeed. */
    uint8_t* ptr_x2 = (uint8_t*)allocate(&options, 10);
    uint8_t* ptr_y2 = (uint8_t*)allocate(&options, 10);
    TEST_EXPECT(nullptr != ptr_x2);
    TEST_EXPECT(nullptr != ptr_y2);

    /* since the bump allocator was reset, the pointers should line up. */
    TEST_EXPECT(ptr_x1 == ptr_x2);
    TEST_EXPECT(ptr_y1 == ptr_y2);

    /* clean up. */
    dispose(allocator_options_disposable_handle(&options));
}
