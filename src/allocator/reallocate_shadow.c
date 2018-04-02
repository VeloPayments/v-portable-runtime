/**
 * \file reallocate_shadow.c
 *
 * Shadow library implementation of allocator.reallocate.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>
#include <string.h>

/* only use for shadow library. */
#ifdef MODEL_CHECK_vpr_allocator_shadowed

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
    size_t new_size)
{
    size_t copy_size;
    void* new_region;

    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));
    MODEL_ASSERT(new_size > 0);
    /* don't overwrite the new memory region. */
    copy_size = old_size > new_size ? new_size : old_size;
    MODEL_ASSERT(copy_size <= new_size);
    MODEL_ASSERT(copy_size <= old_size);

    /* if the user has defined a reallocate method, use it. */
    if (options->allocator_reallocate)
    {
        return options->allocator_reallocate(options->context, mem, old_size,
            new_size);
    }
    /* otherwise, reallocate in terms of allocate and deallocate */
    else
    {
        /* use the underlying allocator to allocate the new region. */
        new_region = options->allocator_allocate(options->context, new_size);
        if (new_region)
        {
            /* only perform the copy-free operation if new_region is valid. */
            memcpy(new_region, mem, copy_size);
            options->allocator_release(options->context, mem);
        }

        /* return the reallocated memory location or 0 if allocation failed. */
        return new_region;
    }
}

#endif /*defined(MODEL_CHECK_vpr_allocator_shadowed)*/
