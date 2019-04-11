/**
 * \file hashmap_put_var_key.c
 *
 * Implementation of hashmap_put_var_key.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

int hashmap_put_var_key(hashmap_t* hmap, uint8_t* key, size_t key_len,
    void* val)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(key_len > 0);
    MODEL_ASSERT(NULL != val);

    // convert variable length key to a 64 bit key
    uint64_t converted_key = hmap->options->hash_func(key, key_len);

    // store using hashmap_put
    return hashmap_put(hmap, converted_key, val);
}
