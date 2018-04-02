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

/* this is the real implementation. */
#ifndef MODEL_CHECK_vpr_malloc_allocator_shadowed

/**
 * \brief This method initializes the provided allocator_options_t structure so
 * that it uses malloc() and free() for managing memory.
 *
 * After malloc_allocator_options_init() is called, the caller owns the
 * ::allocator_options_t structure and must dispose of it by calling dispose().
 *
 * \param options       The allocation options structure to initialize.  Owned
 *                      by the caller.
 */
void malloc_allocator_options_dispose(allocator_options_t* options)
{
    /* the allocator options structure should now be valid. */
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));

    /* clean up this structure */
    MODEL_EXEMPT(memset(options, 0, sizeof(allocator_options_t)));
}

#endif /*!defined(MODEL_CHECK_vpr_malloc_allocator_shadowed)*/
