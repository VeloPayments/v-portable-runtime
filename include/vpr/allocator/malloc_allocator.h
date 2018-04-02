/**
 * \file malloc_allocator.h
 *
 * This header provides malloc_allocator_options_init(), which initializes an
 * allocator_options_t structure so that it can be used with malloc().
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_ALLOCATOR_MALLOC_ALLOCATOR_HEADER_GUARD
#define VPR_ALLOCATOR_MALLOC_ALLOCATOR_HEADER_GUARD

#include <vpr/allocator.h>
#include <stdlib.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \brief This method initializes the provided allocator_options_t structure so
 * that it uses malloc() and free() for managing memory.
 *
 * After malloc_allocator_options_init() is called, the caller owns the
 * ::allocator_options_t structure and must dispose of it by calling dispose().
 *
 * \param options       The allocation options structure to initialize.  Owned
 *                      by the caller.
 */
void malloc_allocator_options_init(allocator_options_t* options);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_ALLOCATOR_MALLOC_ALLOCATOR_HEADER_GUARD
