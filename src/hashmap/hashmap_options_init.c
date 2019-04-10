/**
 * \file hashmap_options_init.c
 *
 * Implementation of hashmap_options_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <stdbool.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void hashmap_item_copy(void*, const void*, size_t);
static void hashmap_item_dispose(allocator_options_t*, void*);

/**
 * \brief Initialize hashmap options for a POD data type.
 *
 * When the function completes successfully, the caller owns this
 * ::hashmap_options_t instance and must dispose of it by calling
 * dispose() when it is no longer needed.
 *
 * \param options             The hashmap options to initialize.
 * \param alloc_opts          The allocator options to use.
 * \param capacity            The number of buckets to allocate.
 * \param copy_on_insert      If true, data will be copied before adding to the
 *                            hashmap, leaving the caller with ownership of
 *                            the original data.  The hashmap assumes
 *                            ownership of the copied data and will free it
 *                            when disposed of.
 *                            If false, the release_on_dispose argument
 *                            dictates what happens when the hashmap is
 *                            disposed of.
 * \param item_size           The size in bytes of an individual data item.
 *                            This parameter is ignored when copy_on_insert is
 *                            false, but must be a positive integer value if
 *                            copy_on_insert is true.
 * \param release_on_dispose  This parameter is ignored if copy_on_insert is
 *                            true.  When copy_on_insert is false, this
 *                            argument determines whether memory storing data
 *                            items is released when the hashmap is disposed
 *                            of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    uint32_t capacity, bool copy_on_insert, size_t item_size,
    bool release_on_dispose)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(NULL != alloc_opts->allocator_release);
    MODEL_ASSERT(capacity > 0);
    MODEL_ASSERT(0 != item_size);

    return hashmap_options_init_ex(options, alloc_opts, capacity,
        copy_on_insert ? &hashmap_item_copy : NULL,
        item_size,
        (copy_on_insert || release_on_dispose) ? &hashmap_item_dispose : NULL);
}

/**
* \brief The copy method to use when adding items to this hashmap.
*
* \param destination   The destination to which this value will be copied.
* \param source        The source data to be copied.
* \param size          The size of the data being copied.
*/
static void hashmap_item_copy(void* destination, const void* source, size_t size)
{
    MODEL_ASSERT(NULL != destination);
    MODEL_ASSERT(NULL != source);
    MODEL_ASSERT(size > 0);

    memcpy(destination, source, size);
}

/**
 * \brief The dispose method to use when disposing of an item in this hashmap.
 *
 * \param alloc_opts    The allocator options to use.
 * \param item          The item to be disposed of.
 */
static void hashmap_item_dispose(allocator_options_t* alloc_opts, void* item)
{
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(NULL != item);

    release(alloc_opts, item);
}
