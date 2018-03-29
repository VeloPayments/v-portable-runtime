/**
 * \file error_codes.h
 *
 * Error codes for vpr.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_ERROR_CODES_HEADER_GUARD
#define VPR_ERROR_CODES_HEADER_GUARD

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \defgroup error codes for the Velo Portable Runtime.
 *
 * @{
 */
#define VPR_STATUS_SUCCESS 0x0000
#define VPR_ERROR_DYNAMIC_ARRAY_APPEND_NO_RESERVE 0x1100
#define VPR_ERROR_DYNAMIC_ARRAY_INIT_ALLOCATION_FAILED 0x1101
#define VPR_ERROR_DYNAMIC_ARRAY_SORT_GENERAL 0x1102
#define VPR_ERROR_DYNAMIC_ARRAY_SORT_ALLOCATION_FAILED 0x1103
#define VPR_ERROR_DYNAMIC_ARRAY_GROW_INVALID_ARGUMENT 0x1104
#define VPR_ERROR_DYNAMIC_ARRAY_GROW_ALLOCATION_FAILED 0x1105
/**
 * @}
 */

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_ERROR_CODES_HEADER_GUARD
