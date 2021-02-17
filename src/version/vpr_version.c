/**
 * \file version/vpr_version.c
 *
 * Return the version of VPR as a constant string.
 *
 * \copyright 2021 Velo Payments, Inc.  All rights reserved.
 */

#include <config.h>
#include <vpr/version.h>

/**
 * \brief Return the version string for the VPR library.
 *
 * \returns a const version string for this library.
 */
const char* vpr_version()
{
    return VPR_VERSION;
}
