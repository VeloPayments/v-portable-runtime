/**
 * \file bloom_filter_contains_item.c
 *
 * Implementation of bloom_filter_contains_item.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/bloom_filter.h>

/**
 * \brief Query a bloom filter to determine if an item has been added.
 *
 * \param bloom             The bloom filter.
 * \param data              The null terminated data to query the filter for.
 *
 * \returns a boolean value indicating if the data is present in the filter.
 */
_Bool bloom_filter_contains_item(bloom_filter_t* bloom, const void* data)
{
    MODEL_ASSERT(NULL != bloom);
    MODEL_ASSERT(NULL != bloom->options);
    MODEL_ASSERT(NULL != bloom->bitmap);
    MODEL_ASSERT(NULL != data);

    // compute the hash of the data for each hash function, and
    // check the appropriate bit in the filter
    for (int n = 0; n < bloom->options->num_hash_functions; n++)
    {
        unsigned int hash_val = bloom_filter_hash(bloom->options, data, n);

        // check the bit corresponding to this hash value
        uint8_t* ptr = (uint8_t*)bloom->bitmap;
        if (!(ptr[hash_val / 8] & (1 << (hash_val % 8))))
        {
            // if that bit isn't set, the data item is definitely
            // not in the set
            return false;
        }
    }

    // we didn't find any evidence that it's not in the set, so
    // it's probably in the set.

    return true;
}
