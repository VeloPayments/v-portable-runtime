/**
 * \file allocate_shadow.c
 *
 * Shadow library implementation of allocator.allocate.
 *
 * \copyright 2017-2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

/* only use for shadow library. */
#ifdef MODEL_CHECK_vpr_allocator_shadowed

/**
 * Allocate memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param size          Size of memory region to allocate, in bytes.
 */
void* allocate(allocator_options_t* options, size_t size)
{
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));
    MODEL_ASSERT(size > 0);

    return options->allocator_allocate(options->context, size);
}

#endif /*defined(MODEL_CHECK_vpr_allocator_shadowed)*/
