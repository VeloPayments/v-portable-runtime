/**
 * \file malloc_allocator_options_dispose.c
 *
 * Shadow library implementation of malloc_allocator_options_dispose.
 *
 * \copyright 2017-2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_malloc_allocator_shadowed

/**
 * This method disposes of the underlying allocator provided by the given
 * allocator_options_t structure that was initialized by
 * malloc_allocator_options_init.
 *
 * \note this dispose method DOES NOT FREE the options structure.  This
 *       structure is owned by the caller.
 *
 * \param options       The allocation options structure to dispose.  Owned
 *                      by the caller.
 */
void malloc_allocator_options_dispose(allocator_options_t* options)
{
    /* the allocator options structure should now be valid. */
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));

    /* clean up this structure */
    MODEL_EXEMPT(memset(options, 0, sizeof(allocator_options_t)));
}

#endif /*defined(MODEL_CHECK_vpr_malloc_allocator_shadowed)*/
