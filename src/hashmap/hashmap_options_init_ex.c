/**
 * \file hashmap_options_init_ex.c
 *
 * Implementation of hashmap_options_init_ex.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <math.h>
#include <string.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void hashmap_simple_dispose(void*);

/**
 * \brief Initialize a hashmap
 *
 * \param options                  The hashmap options to initialize.
 * \param alloc_opts               The allocator options to use.
 * \param capacity                 The number of buckets to allocate.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init_ex(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    unsigned int capacity)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(capacity > 0);

    options->hdr.dispose = &hashmap_simple_dispose;
    options->alloc_opts = alloc_opts;
    options->capacity = capacity;

    return VPR_STATUS_SUCCESS;
}

/**
 * Dispose of the options structure.  Nothing special needs to be done.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void hashmap_simple_dispose(void* UNUSED(poptions))
{
    MODEL_ASSERT(poptions != NULL);
}
