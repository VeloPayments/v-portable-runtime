/**
 * \file hash_func.c
 *
 * A collection of hash functions.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hash_func.h>

/**
 * An implementation of the sdbm hash algorithm
 *
 * \param data               The data to hash
 * \param len                The length of the data to be hashed.
 *
 * \returns The 64 bit hash signature of the input data
 */
uint64_t sdbm(const void* data, size_t len)
{
    uint8_t* ptr = (uint8_t*)data;

    uint64_t hash = 5381;

    MODEL_ASSUME(len < 3);
    for (unsigned int i = 0; i < len; i++)
    {
        uint8_t c = *ptr++;
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

/**
 * An implementation of the jenkins hash algorithm
 *
 * \param data               The data to hash
 * \param len                The length of the data to be hashed.
 *
 * \returns The 64 bit hash signature of the input data
 */
uint64_t jenkins(const void* data, size_t len)
{
    uint8_t* ptr = (uint8_t*)data;

    uint64_t hash = 0;

    MODEL_ASSUME(len < 3);
    for (unsigned int i = 0; i < len; i++)
    {
        hash += *ptr++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
