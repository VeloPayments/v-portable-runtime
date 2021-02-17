/**
 * \file vpr/version.h
 *
 * \brief Get a link time version string for the vpr library.
 *
 * \copyright 2021 Velo Payments, Inc.  All rights reserved.
 */

#ifndef  VPR_VERSION_HEADER_GUARD
# define VPR_VERSION_HEADER_GUARD

/* make this header C++ friendly. */
#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * \brief Return the version string for the VPR library.
 *
 * \returns a const version string for this library.
 */
const char* vpr_version();

/* make this header C++ friendly. */
#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*VPR_VERSION_HEADER_GUARD*/
