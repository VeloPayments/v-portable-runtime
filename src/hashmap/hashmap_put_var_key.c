/**
 * \file hashmap_put_var_key.c
 *
 * Implementation of hashmap_put_var_key.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

/**
 * \brief Add an item to a hashmap.
 *
 * Add a data item to the hashmap using a variable length key.
 *
 * \param hmap              The hashmap to add the item to.
 * \param key               A unique key that serves as an identifier for the
 *                          data item.
 * \param key_len           The length of the key.
 * \param val               Opaque pointer to the data item.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - non-zero code on failure.
 */
int hashmap_put_var_key(hashmap_t* hmap, uint8_t* key, size_t key_len,
    void* val)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(key_len > 0);
    MODEL_ASSERT(NULL != val);

    // convert variable length key to a 64 bit key
    uint64_t key64 = hmap->options->hash_func(key, key_len);

    // store using hashmap_put
    return hashmap_put(hmap, key64, val);
}
