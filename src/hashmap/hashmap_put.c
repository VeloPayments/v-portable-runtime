/**
 * \file hashmap_put.c
 *
 * Implementation of hashmap_put.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/parameters.h>

/**
 * \brief Add an item to a hashmap.
 *
 * Add a data item to the hashmap.  The 64 bit key should be a random value
 * to minimize chaining, which will decrease performance.
 *
 * \param hmap              The hashmap to add the item to.
 * \param key               A unique key that serves as an identifier for the
 *                          data item.
 * \param val               Opaque pointer to the data item.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - non-zero code on failure.
 */
int hashmap_put(hashmap_t* hmap, uint64_t key, void* val)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != hmap->options);
    MODEL_ASSERT(hmap->options->capacity > 0);
    MODEL_ASSERT(NULL != val);

    uint8_t* keyptr = (uint8_t*)&key;

    return hashmap_put_var_key(hmap, keyptr, sizeof(uint64_t), val);
}
