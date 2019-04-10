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
 * \brief Initialize hashmap options for a custom data type.
 *
 * This method allows the user to override the copy and dispose methods to
 * manage non-POD data types and complex data structures.
 *
 * When the function completes successfully, the caller owns this
 * ::hashmap_options_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The hashmap options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param capacity          The number of buckets to allocate.
 * \param copy_method       Optional - The method to use to copy elements.
 *                          If provided then elements are copied into
 *                          separate memory as they are added to the list.
 * \param item_size         Optional (when copy_method is NULL).
 *                          The size of an individual data item.
 * \param dispose_method    Optional - The method to use to dispose of data
 *                          items.
 *                          If provided then this method is invoked on each
 *                          data item when the list is disposed of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init_ex(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    uint32_t capacity, hashmap_item_copy_t copy_method,
    size_t item_size, hashmap_item_dispose_t dispose_method)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(NULL != alloc_opts->allocator_release);
    MODEL_ASSERT(capacity > 0);
    MODEL_ASSERT(NULL == copy_method || item_size > 0);

    options->hdr.dispose = &hashmap_simple_dispose;
    options->alloc_opts = alloc_opts;
    options->capacity = capacity;
    options->hashmap_item_copy = copy_method;
    options->item_size = item_size;
    options->hashmap_item_dispose = dispose_method;


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
