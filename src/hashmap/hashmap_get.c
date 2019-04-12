/**
 * \file hashmap_get.c
 *
 * Implementation of hashmap_get.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

/**
 * \brief Retrieve a value from a hashmap using a variable length key.
 *
 * \param hmap              The hashmap to query
 * \param key               The key identifying the item.
 * \param key_len           The length of the key in bytes.
 *
 * \returns an opaque pointer to the item, or NULL if it wasn't found.
 */
void* hashmap_get(hashmap_t* hmap, uint8_t* key, size_t key_len)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(key_len > 0);

    // figure out which bucket
    uint64_t hashed_key = hmap->options->hash_func(key, key_len);
    uint32_t bucket = hashed_key % hmap->options->capacity;

    // get the doubly linked list from the bucket
    void** buckets = hmap->buckets;
    doubly_linked_list_t* dllptr = (doubly_linked_list_t*)buckets[bucket];

    // if there is no doubly linked list there is no value to return
    if (NULL == dllptr)
    {
        return NULL;
    }

    // search for the key within that list
    doubly_linked_list_element_t* element = dllptr->first;
    while (element != NULL)
    {
        hashmap_entry_t* hmap_entry = (hashmap_entry_t*)element->data;
        if (hmap_entry->hashed_key == hashed_key)
        {
            // the hashed keys match, which almost guarantees a match.  If an
            // equality function was supplied, use it as a verification.
            if (NULL == hmap->options->equals_func || hmap->options->equals_func(key, hmap_entry->val))
            {
                return hmap_entry->val;
            }
        }

        element = element->next;
    }


    return NULL;
}
