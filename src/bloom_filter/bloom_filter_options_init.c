/**
 * \file bloom_filter_options_init.c
 *
 * Implementation of doubly_linked_list_options_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static uint64_t bloom_filter_hash_func_1(const void*);
static uint64_t bloom_filter_hash_func_2(const void*);

int bloom_filter_options_init(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, size_t size)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(size > 0);

    return bloom_filter_options_init_ex(options, alloc_opts, size,
        &bloom_filter_hash_func_1, &bloom_filter_hash_func_2);
}


/**
 * The hash function to use when testing membership using the bloom filter
 *
 * \param                   The null terminated data to hash
 *
 * \returns The hash signature of the input data
 */
static uint64_t bloom_filter_hash_func_1(const void* UNUSED(data))
{
    // TODO
    return 0;
}

static uint64_t bloom_filter_hash_func_2(const void* UNUSED(data))
{
    // TODO
    return 0;
}
