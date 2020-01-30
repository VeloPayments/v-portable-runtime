/**
 * \file function_decl.h
 *
 * \brief Function declaration macros for VPR.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_FUNCTION_DECL_HEADER_GUARD
#define VPR_FUNCTION_DECL_HEADER_GUARD

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/*
 * With GCC 3.4 and clang, we can force a warning / error if the return value
 * of a function isn't checked.
 */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#if (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ > 3)
#define VPR_DECL_MUST_CHECK __attribute__((warn_unused_result))
#endif
#endif

/*
 * For other compilers, skip this check.
 */
#ifndef VPR_DECL_MUST_CHECK
#define VPR_DECL_MUST_CHECK
#endif

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_FUNCTION_DECL_HEADER_GUARD
