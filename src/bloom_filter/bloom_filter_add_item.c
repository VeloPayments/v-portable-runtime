/**
 * \file bloom_filter_add_item.c
 *
 * Implementation of bloom_filter_add_item.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <stdio.h>  // TODO: temporary
#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

int bloom_filter_add_item(bloom_filter_t* bloom, const void* data)
{
    MODEL_ASSERT(NULL != bloom);
    MODEL_ASSERT(NULL != bloom->options);
    MODEL_ASSERT(NULL != bloom->bitmap);
    MODEL_ASSERT(NULL != data);

    printf("adding item: %s\n", (const char*)data);
    // compute the hash of the data for each hash function, and
    // set the appropriate bit in the filter
    //for (int n=0; n<bloom->options->num_hash_functions; n++)
    //{
    uint64_t hash_val = bloom_filter_hash(bloom->options, data, 0);
    printf("    hv: %lu\n", hash_val);

    // set the bit corresponding to hash_val to 1
    uint8_t* ptr = (uint8_t*)bloom->bitmap;
    printf("ptr: %p\n", ptr);
    //ptr[hash_val >> 3] |= (uint8_t)1 << (hash_val & 7);
    //}

    return VPR_STATUS_SUCCESS;
}
