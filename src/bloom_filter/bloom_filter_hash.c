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

// n is the "nth hash"
// returned value will be the bit in the filter to set
unsigned int bloom_filter_hash(bloom_filter_options_t* options, const void* data,
    int n)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != options->hash_function_1);
    MODEL_ASSERT(NULL != options->hash_function_2);
    MODEL_ASSERT(options->size_in_bytes > 0);
    MODEL_ASSERT(NULL != data);
    MODEL_ASSERT(n >= 0);

    unsigned int hv1 = options->hash_function_1(data);
    unsigned int hv2 = options->hash_function_2(data);

    // the returned answer needs to be modulo the number of bits in the filter
    unsigned int m = options->size_in_bytes * 8;

    return (hv1 + n * hv2) % m;
}
