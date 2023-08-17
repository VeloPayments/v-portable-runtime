/**
 * \file test_vpr_uuid_from_string.cpp
 *
 * Unit tests for vpr_uuid_from_string.
 *
 * \copyright 2020-2023 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <string.h>
#include <vpr/uuid.h>

TEST_SUITE(vpr_uuid_from_string);

/**
 * Test that a basic UUID can be read from a string.
 */
TEST(basic_uuid)
{
    const char* INPUT_STRING = "a99b994a-36e3-4d13-98ba-74748a1f5e89";
    const vpr_uuid EXPECTED_UUID = {
        .data = { 0xa9, 0x9b, 0x99, 0x4a, 0x36, 0xe3, 0x4d, 0x13,
            0x98, 0xba, 0x74, 0x74, 0x8a, 0x1f, 0x5e, 0x89 }
    };
    vpr_uuid value;

    /* do uuid from string conversion. */
    int retval = vpr_uuid_from_string(&value, INPUT_STRING);

    /* verify results. */
    TEST_ASSERT(VPR_STATUS_SUCCESS == retval);
    TEST_EXPECT(
        0 == memcmp(EXPECTED_UUID.data, value.data, sizeof(value.data)));
}
