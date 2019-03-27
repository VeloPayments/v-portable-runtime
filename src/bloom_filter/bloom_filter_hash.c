/**
 * \file bloom_filter_hash.c
 *
 * Implementation of bloom_filter_hash.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/bloom_filter.h>

/**
 * \brief Hash an input value to determine which bit of the filter to set.
 *
 * \param options           The bloom filter options to use for this instance.
 * \param data              The data to hash.
 * \param len               The length of the data to hash.
 * \param n                 The round of hashing represented by this operation.
 *                          If the filter has K hash functions, this value
 *                          should be in the range [0, K).
 *
 * \returns The position of the bit in the filter to set to 1.
 */
unsigned int bloom_filter_hash(bloom_filter_options_t* options,
    const void* data, size_t len, unsigned int n)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != options->hash_function_1);
    MODEL_ASSERT(NULL != options->hash_function_2);
    MODEL_ASSERT(options->size_in_bytes > 0);
    MODEL_ASSERT(NULL != data);
    MODEL_ASSERT(len > 0);
    MODEL_ASSERT(n >= 0);

    uint64_t hv1 = options->hash_function_1(data, len);

    uint64_t hv2 = options->hash_function_2(data, len);

    // the returned answer needs to be modulo the number of bits in the filter
    unsigned int m = options->size_in_bytes * 8;

    return (hv1 + n * hv2) % m;
}
