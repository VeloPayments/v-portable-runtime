/**
 * \file release.c
 *
 * Implementation of allocator.release.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

/**
 * \brief Release memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param mem           The memory to release.
 */
void release(allocator_options_t* options, void* mem)
{
    MODEL_ASSERT(prop_allocator_valid(options));
    MODEL_ASSERT(mem != NULL);

    options->allocator_release(options->context, mem);
}
