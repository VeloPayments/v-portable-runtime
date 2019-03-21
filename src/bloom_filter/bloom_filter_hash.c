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

uint64_t bloom_filter_hash(bloom_filter_options_t* options, void* data,
    uint64_t n, uint64_t m)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != options->hash_function_1);
    MODEL_ASSERT(NULL != options->hash_function_2);
    MODEL_ASSERT(NULL != data);
    MODEL_ASSERT(n >= 0);
    MODEL_ASSERT(m > 0);

    return (options->hash_function_1(data) +
               n * options->hash_function_2(data)) %
        m;
}
