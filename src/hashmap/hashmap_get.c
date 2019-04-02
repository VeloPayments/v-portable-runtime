/**
 * \file hashmap_get.c
 *
 * Implementation of hashmap_get.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

/**
 * \brief Retrieve a data item from a hashmap.
 *
 * Query a hashmap using a key that uniquely identifies a data item.
 *
 * \param hmap              The hashmap to query
 * \param key               The key identifying the item.
 *
 * \returns an opaque pointer to the item, or NULL if it wasn't found.
 */
void* hashmap_get(hashmap_t* hmap, uint64_t key)
{
    MODEL_ASSERT(NULL != hmap);

    // figure out which bucket
    unsigned int bucket = key % hmap->options->capacity;

    // get the doubly linked list from the bucket
    doubly_linked_list_t** dll =
        bucket + (doubly_linked_list_t**)hmap->buckets;

    // if there is no doubly linked list there is no value to return
    if (NULL == *dll)
    {
        return NULL;
    }

    // search for the key within that list
    doubly_linked_list_element_t* element = (*dll)->first;
    while (element != NULL)
    {
        hashmap_entry_t* hmap_entry = (hashmap_entry_t*)element->data;
        if (hmap_entry->key == key)
        {
            return hmap_entry->val;
        }

        element = element->next;
    }


    return NULL;
}
