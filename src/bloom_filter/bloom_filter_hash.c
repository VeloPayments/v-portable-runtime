/**
 * \file bloom_filter_hash.c
 *
 * Implementation of bloom_filter_hash.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

// n is the "nth hash"
// returned value will be [0, max_size)
uint64_t bloom_filter_hash(bloom_filter_options_t* options, const void* data,
    int n)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != options->hash_function_1);
    MODEL_ASSERT(NULL != options->hash_function_2);
    MODEL_ASSERT(options->size_in_bytes > 0);
    MODEL_ASSERT(NULL != data);
    MODEL_ASSERT(n >= 0);

    // create a mask to limit the size of the returned value to something
    // that will fit in the specified size of this bloom filter
    // TODO: consider putting this in the options as part of initialization
    uint64_t m = ((uint64_t)1 << (options->size_in_bytes * 8)) - 1;

    return (options->hash_function_1(data) +
               n * options->hash_function_2(data)) &
        m;
}
