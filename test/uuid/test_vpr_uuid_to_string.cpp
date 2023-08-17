/**
 * \file test_vpr_uuid_to_string.cpp
 *
 * Unit tests for vpr_uuid_to_string.
 *
 * \copyright 2020-2023 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/uuid.h>

TEST_SUITE(vpr_uuid_to_string);

/**
 * Test that the base case works on the zero uuid.
 */
TEST(base_case_zero)
{
    const vpr_uuid ZERO_UUID = {
        .data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
    };
    const char* EXPECTED_UUID = "00000000-0000-0000-0000-000000000000";
    char* value = nullptr;
    allocator_options_t alloc_opts;

    /* create malloc allocator. */
    malloc_allocator_options_init(&alloc_opts);

    /* do uuid to string conversion. */
    int retval = vpr_uuid_to_string(&value, &alloc_opts, &ZERO_UUID);

    /* verify results. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == retval);
    TEST_ASSERT(nullptr != value);
    TEST_EXPECT(0 == strcmp(value, EXPECTED_UUID));

    /* cleanup. */
    release(&alloc_opts, value);
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

/**
 * Test that a basic UUID can be converted to a string.
 */
TEST(basic_uuid)
{
    const vpr_uuid INPUT_UUID = {
        .data = { 0xf7, 0xe7, 0x01, 0x22, 0x66, 0x53, 0x4a, 0x5c,
            0xb3, 0xe9, 0xfe, 0xa6, 0x29, 0x34, 0x3c, 0x19 }
    };
    const char* EXPECTED_STRING = "f7e70122-6653-4a5c-b3e9-fea629343c19";
    char* value = nullptr;
    allocator_options_t alloc_opts;

    /* create malloc allocator. */
    malloc_allocator_options_init(&alloc_opts);

    /* do uuid to string conversion. */
    int retval = vpr_uuid_to_string(&value, &alloc_opts, &INPUT_UUID);

    /* verify results. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == retval);
    TEST_ASSERT(nullptr != value);
    TEST_EXPECT(0 == strcmp(value, EXPECTED_STRING));

    /* cleanup. */
    release(&alloc_opts, value);
    dispose(allocator_options_disposable_handle(&alloc_opts));
}
