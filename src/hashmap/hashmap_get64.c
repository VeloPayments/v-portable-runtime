/**
 * \file hashmap_get64.c
 *
 * Implementation of hashmap_get64.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

/**
 * \brief Retrieve a data item from a hashmap using a 64 bit key.
 *
 * Query a hashmap using a key that uniquely identifies a data item.
 *
 * \param hmap              The hashmap to query
 * \param key               The 64 bit key identifying the item.
 *
 * \returns an opaque pointer to the item, or NULL if it wasn't found.
 */
void* hashmap_get64(hashmap_t* hmap, uint64_t key)
{
    MODEL_ASSERT(NULL != hmap);

    uint8_t* keyptr = (uint8_t*)&key;

    return hashmap_get(hmap, keyptr, sizeof(uint64_t));
}
