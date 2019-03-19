/**
 * \file bloom_filter_init.c
 *
 * Implementation of bloom_filter_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

//forward decls
static void bloom_filter_dispose(void*);

/**
 * \brief Initialize a bloom filter
 *
 * This method allows for the creation of a bloom filter.  Once initialized,
 * the list will have zero elements.
 *
 * \param options           The bloom filter options to use for this instance.
 * \param dll               The bloom filter to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int bloom_filter_init(bloom_filter_options_t* options, bloom_filter_t* bloom)
{
    //the bloom filter structure must be non-null
    MODEL_ASSERT(NULL != bloom);

    //sanity checks on options
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(options->size > 0);
    MODEL_ASSERT(NULL != options->alloc_opts);

    bloom->hdr.dispose = &bloom_filter_dispose;
    bloom->options = options;

    // allocate the bitmap
    bloom->bitmap = (void*)allocate(bloom->options->alloc_opts,
        bloom->options->size);
    if (NULL == bloom->bitmap)
    {
        return VPR_ERROR_BLOOM_BITMAP_ALLOCATION_FAILED;
    }

    // clear the bitmap
    memset(bloom->bitmap, 0, bloom->options->size);


    //success
    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of a bloom filter.
 *
 * \param pbloom        An opaque pointer to the bloom filter.
 */
void bloom_filter_dispose(void* pbloom)
{
    MODEL_ASSERT(NULL != pbloom);

    bloom_filter_t* bloom = (bloom_filter_t*)pbloom;

    MODEL_ASSERT(NULL != bloom->options);
    MODEL_ASSERT(NULL != bloom->options->alloc_opts);
    MODEL_ASSERT(NULL != bloom->bitmap);

    release(bloom->options->alloc_opts, bloom->bitmap);
}
