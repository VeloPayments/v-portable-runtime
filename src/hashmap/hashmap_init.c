/**
 * \file hashmap_init.c
 *
 * Implementation of hashmap_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

//forward decls
static void hashmap_dispose(void*);

/**
 * \brief Initialize a hashmap.
 *
 * This method allows for the creation of a hashmap.  Once initialized the
 * hashmap will be empty.

 * When the function completes successfully, the caller owns this
 * ::hashmap_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The hashmap options to use for this instance.
 * \param hmap              The hashmap to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_HASHMAP_ALLOCATION_FAILED if memory could not
 *        be allocated for the hashmap
 */
int hashmap_init(hashmap_options_t* options, hashmap_t* hmap)
{
    //the hashmap structure must be non-null
    MODEL_ASSERT(NULL != hmap);

    //sanity checks on options
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != options->alloc_opts);

    hmap->hdr.dispose = &hashmap_dispose;
    hmap->options = options;

    // allocate the space for the hashmap
    hmap->buckets = (void*)allocate(
        hmap->options->alloc_opts,
        hmap->options->capacity * sizeof(void*));
    if (NULL == hmap->buckets)
    {
        return VPR_ERROR_HASHMAP_ALLOCATION_FAILED;
    }

    // clear the hashmap - all buckets contain a NULL pointer
    uint8_t* ptr = hmap->buckets;
    for (unsigned int i = 0; i < hmap->options->capacity; i++)
    {
        *(ptr + i) = 0;
    }

    // this hashmap has no elements
    hmap->elements = 0u;

    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of a hashmap.
 *
 * \param phmap        An opaque pointer to the hashmap.
 */
void hashmap_dispose(void* phmap)
{
    MODEL_ASSERT(NULL != phmap);

    hashmap_t* hmap = (hashmap_t*)phmap;

    MODEL_ASSERT(NULL != hmap->options);
    MODEL_ASSERT(NULL != hmap->options->alloc_opts);

    // TODO: may need to release data items.  (Get model check to fail first.)

    release(hmap->options->alloc_opts, hmap->buckets);
}
