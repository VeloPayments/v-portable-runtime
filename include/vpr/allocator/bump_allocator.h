/**
 * \file bump_allocator.h
 *
 * \brief Bump allocator implementation.
 *
 * This header provides bump_allocator_options_init(), which initializes an
 * allocator_options_t structure so that it can be used as a bump allocator
 * backed by a user-defined buffer.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_ALLOCATOR_BUMP_ALLOCATOR_HEADER_GUARD
#define VPR_ALLOCATOR_BUMP_ALLOCATOR_HEADER_GUARD

#include <vpr/allocator.h>
#include <vpr/function_decl.h>
#include <stdlib.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \defgroup BUMPControlKeys Control keys for controlling the bump allocator.
 *
 * \brief These control keys are used to control the bump allocator.
 *
 * @{
 */

/**
 * \brief The ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET key resets the bump
 * allocator, forcibly freeing all memory underneath.
 */
#define ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET 0x00010001

/**
 * @}
 */

/**
 * \brief Constant value representing the minimum size needed for a backing
 * buffer.
 */
extern const size_t bump_allocator_minimum_buffer_size;

/**
 * \brief This method initializes the provided allocator_options_t structure so
 * that it is backed by a user-defined buffer of a given size.
 *
 * After bump_allocator_options_init() is called, the caller owns the
 * ::allocator_options_t structure and must dispose of it by calling dispose().
 * Furthermore, the caller RETAINS ownership of the backing buffer.  The user
 * cannot use this backing buffer until this allocator is disposed, and after it
 * is disposed, the user can release this buffer as appropriate.
 *
 * \param options       The allocation options structure to initialize.  Owned
 *                      by the caller.
 * \param buffer        The backing buffer used by this bump allocator to
 *                      allocate memory.
 * \param size          The size of the backing buffer.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS on success.
 *      - \ref VPR_ERROR_BUMP_ALLOCATOR_TINY_BUFFER if the buffer is too small
 *        for the internal record keeping needed.
 *      - a non-zero error code indicating failure.
 */
int VPR_DECL_MUST_CHECK
bump_allocator_options_init(
    allocator_options_t* options, void* buffer, size_t size);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_ALLOCATOR_BUMP_ALLOCATOR_HEADER_GUARD
