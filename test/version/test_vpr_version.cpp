/**
 * \file test_vpr_version.cpp
 *
 * Unit tests for vpr_version.
 *
 * \copyright 2021 Velo-Payments, Inc.  All rights reserved.
 */

#include <config.h>
#include <vpr/version.h>

/* DISABLED GTEST */
#if 0
TEST(vpr_version_test, verify_version_information_set)
{
    const char* version = vpr_version();

    ASSERT_NE(nullptr, version);
    EXPECT_STREQ(VPR_VERSION, version);
}
#endif
