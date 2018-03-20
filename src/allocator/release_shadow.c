/**
 * \file release_shadow.c
 *
 * Shadow library implementation of allocator.release.
 *
 * \copyright 2017-2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

/* only use for shadow library. */
#ifdef MODEL_CHECK_vpr_allocator_shadowed

/**
 * Release memory using the given allocator_options_t structure.
 *
 * \param options       Allocator options to use when allocating memory.
 * \param mem           The memory to release.
 */
void release(allocator_options_t* options, void* mem)
{
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));
    MODEL_ASSERT(mem != NULL);

    options->allocator_release(options->context, mem);
}

#endif /*defined(MODEL_CHECK_vpr_allocator_shadowed)*/
