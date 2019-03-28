/**
 * \file hashmap_options_init.c
 *
 * Implementation of hashmap_options_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

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
int hashmap_options_init(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    unsigned int capacity)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(capacity > 0);

    return hashmap_options_init_ex(options, alloc_opts, capacity);
}
