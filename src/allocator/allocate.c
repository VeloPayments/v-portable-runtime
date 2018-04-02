/**
 * \file allocate.c
 *
 * Implementation of allocator.allocate.
 *
 * \copyright 2017-2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

/* this is the real implementation. */
#ifndef MODEL_CHECK_vpr_allocator_shadowed

/**
 * \brief Allocate memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param size          Size of memory region to allocate, in bytes.
 *
 * \returns a pointer to a memory region of the given size or NULL on failure.
 */
void* allocate(allocator_options_t* options, size_t size)
{
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));
    MODEL_ASSERT(size > 0);

    return options->allocator_allocate(options->context, size);
}

#endif /*!defined(MODEL_CHECK_vpr_allocator_shadowed)*/
