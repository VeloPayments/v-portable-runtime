/**
 * \file allocate.c
 *
 * Implementation of allocator.allocate.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

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
    MODEL_ASSERT(prop_allocator_valid(options));
    MODEL_ASSERT(size > 0);

    return options->allocator_allocate(options->context, size);
}
