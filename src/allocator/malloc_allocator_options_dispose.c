/**
 * \file malloc_allocator_options_dispose.c
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/parameters.h>

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
void malloc_allocator_options_dispose(allocator_options_t* UNUSED(options))
{
    MODEL_ASSERT(options != NULL);
}
