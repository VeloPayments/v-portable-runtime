/**
 * \file hashmap_get_var_key.c
 *
 * Implementation of hashmap_get_var_key.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

/**
 * \brief Retrieve a data item from a hashmap.
 *
 * Query a hashmap using a variable length key.
 *
 * \param hmap              The hashmap to query
 * \param key               The key identifying the item.
 * \param key_len           The length of the key in bytes.
 *
 * \returns an opaque pointer to the item, or NULL if it wasn't found.
 */
void* hashmap_get_var_key(hashmap_t* hmap, uint8_t* key, size_t key_len)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(key_len > 0);

    // convert variable length key to a 64 bit key
    uint64_t key64 = hmap->options->hash_func(key, key_len);

    return hashmap_get(hmap, key64);
}
