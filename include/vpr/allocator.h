/**
 * \file allocator.h
 *
 * The allocator provides a user-customizable mechanism for managing allocation
 * concerns.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_ALLOCATOR_HEADER_GUARD
#define VPR_ALLOCATOR_HEADER_GUARD

#include <vpr/disposable.h>
#include <stdlib.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \brief This structure contains the options that can be overridden by the user
 * to customize allocator behavior.
 *
 * Code making use of the VPR allocator can use these routines, but is not
 * necessarily required to use all of them except for
 * allocator_options_t::allocator_allocate() and
 * allocator_options_t::allocator_release().
 */
typedef struct allocator_options
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief Allocate memory.
     *
     * \param context       The user-defined context for this call.
     * \param size          The amount of memory to be allocated, in bytes.
     *
     * \returns the allocated memory block, or 0 if memory could not be
     *          allocated.
     */
    void* (*allocator_allocate)(void* context, size_t size);

    /**
     * \brief Release (free) memory previously allocated, or decrement
     * reference, or do nothing, depending upon allocator implementation.
     *
     * \note Users of ::allocator_options_t should assume malloc()/free()
     *       semantics when using allocator_allocate() and allocator_free().
     *       This ensures the maximum level of portability, even if
     *       allocator_release() calls may be NO-OPs when a collector or bump
     *       allocator are used.
     *
     * \param context       The user-defined context for this call.
     * \param mem           The memory to release.
     */
    void (*allocator_release)(void* context, void* mem);

    /**
     * \brief Optional reallocate method.
     *
     * Resize the given memory region to the new size, returning the updated
     * memory region.  This method will attempt to resize the memory region if
     * possible, otherwise, it will allocate a larger memory region and copy the
     * given number of bytes to the newly allocated regino before freeing the
     * previously allocated region.
     *
     * If the new size is smaller than the old size, then only the amount of
     * bytes valid in the new region will be copied.  Alternately, this may not
     * result in a reallocation at all, depending upon the underlying
     * implementation.  The implementation may decide when to truncate and when
     * not to truncate, depending upon how memory is laid out in a particular
     * machine architecture and allocation strategy.
     *
     * If this method is not defined, then the reallocate method defined below
     * will brute-force allocate-copy-free.  In either case, the returned memory
     * location should be used to update any pointers to the previous region, as
     * that region may no longer be valid after this call.
     *
     * \param context       The user-defined context for this call.
     * \param mem           The memory to reallocate.
     * \param old_size      The previous size of the region, used for copying.
     * \param new_size      The newly requested size.
     *
     * \returns the updated memory region, which should be assumed to be
     *          new_size bytes in size.
     */
    void* (*allocator_reallocate)(void* context, void* mem, size_t old_size,
        size_t new_size);

    /**
     * \brief user context for allocator options.
     *
     * This context is passed to each of the above functions when called.
     */
    void* context;

} allocator_options_t;

/**
 * \brief This macro defines the model check property for a valid allocator
 * options structure.
 */
#define MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options) \
    (NULL != (options) && MODEL_PROP_VALID_DISPOSABLE(&((options)->hdr)) && NULL != (options)->allocator_allocate && NULL != (options)->allocator_release && (NULL != (options)->allocator_reallocate || NULL == (options)->allocator_reallocate))

/**
 * \brief Allocate memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param size          Size of memory region to allocate, in bytes.
 *
 * \returns a pointer to a memory region of the given size or NULL on failure.
 */
void* allocate(allocator_options_t* options, size_t size);

/**
 * \brief Release memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param mem           The memory to release.
 */
void release(allocator_options_t* options, void* mem);

/**
 * \brief Reallocate memory using the given allocator_options_t structure.
 *
 * If allocator_reallocate is defined, then this method is used.  Otherwise, a
 * new memory region of the given size is allocated, and MIN(old_bytes,
 * new_bytes) is copied from the old region to the new region.  Finally, the
 * previous memory region is freed and the new region is returned.
 *
 * \param options       Allocator options to use when reallocating memory.
 * \param mem           The memory region to reallocate.
 * \param old_size      The previous size of the region, used for copying.
 * \param new_size      The newly requested size.
 *
 * \returns the updated memory region, which should be assumed to be new_size
 *          bytes in size.  If reallocation fails, then 0 is returned, and
 *          the previous memory region is still valid.
 */
void* reallocate(allocator_options_t* options, void* mem, size_t old_size,
    size_t new_size);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_ALLOCATOR_HEADER_GUARD
