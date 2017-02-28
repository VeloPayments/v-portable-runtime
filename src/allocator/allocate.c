/**
 * \file allocate.c
 *
 * Implementation of allocator.allocate.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

/**
 * Allocate memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param size          Size of memory region to allocate, in bytes.
 */
void* allocate(allocator_options_t* options, size_t size)
{
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(options->allocator_allocate != NULL);
    MODEL_ASSERT(size > 0);

    return options->allocator_allocate(options->context, size);
}
