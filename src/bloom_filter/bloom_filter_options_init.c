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
static uint64_t djb2(const void*);
static uint64_t sdbm(const void*);

int bloom_filter_options_init(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, size_t size)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(size > 0);

    return bloom_filter_options_init_ex(
        options, alloc_opts, size, &djb2, &sdbm);
}


/**
 * An implementation of the djb2 hash algorithm
 *
 * \param                   The null terminated data to hash
 *
 * \returns The 64 bit hash signature of the input data
 */
static uint64_t djb2(const void* data)
{
    uint8_t* ptr = (uint8_t*)data;

    uint64_t hash = 5381;
    uint8_t c;

    while ((c = *ptr++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * An implementation of the sdbm hash algorithm
 *
 * \param                   The null terminated data to hash
 *
 * \returns The 64 bit hash signature of the input data
 */
static uint64_t sdbm(const void* data)
{
    uint8_t* ptr = (uint8_t*)data;

    uint64_t hash = 5381;
    uint8_t c;

    while ((c = *ptr++))
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }


    return hash;
}
