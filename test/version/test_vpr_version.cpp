/**
 * \file test_vpr_version.cpp
 *
 * Unit tests for vpr_version.
 *
 * \copyright 2021-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <config.h>
#include <minunit/minunit.h>
#include <string.h>
#include <vpr/version.h>

TEST_SUITE(vpr_version_test);

TEST(verify_version_information_set)
{
    const char* version = vpr_version();

    TEST_ASSERT(nullptr != version);
    TEST_EXPECT(!strcmp(VPR_VERSION, version));
}
