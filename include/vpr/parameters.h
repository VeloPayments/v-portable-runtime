/**
 * \file parameters.h
 *
 * \brief Helper macros to deal with parameter contracts.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_PARAMETERS_HEADER_GUARD
#define VPR_PARAMETERS_HEADER_GUARD

/**
 * The UNUSED macro is used to annotate a local function's parameter as unused.
 * This is used when implementations of interfaces don't use all parameters.
 */
#ifndef CBMC
#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(X) UNUSED_##x
#endif  //__GNUC__
#else  // defined(CBMC)
#define UNUSED(x) x
#endif

#endif  //VPR_PARAMETERS_HEADER_GUARD
