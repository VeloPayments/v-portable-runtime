/**
 * \file hashmap_put.c
 *
 * Implementation of hashmap_put.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

static doubly_linked_list_t* allocate_dll(
    allocator_options_t* alloc_opts, bool copy_on_insert,
    size_t element_size, bool release_on_dispose);

int hashmap_put(hashmap_t* hmap, uint64_t key, void* val)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != hmap->options);
    MODEL_ASSERT(hmap->options->capacity > 0);
    MODEL_ASSERT(NULL != val);

    // figure out which bucket
    unsigned int bucket = key % hmap->options->capacity;

    // get the linked list in that bucket.  If there is not already one,
    // create one and add it to the bucket now.
    doubly_linked_list_t** dllptr = (doubly_linked_list_t**)(hmap->buckets + bucket);
    if (NULL == *dllptr)
    {
        *dllptr = (doubly_linked_list_t*)allocate_dll(
            hmap->options->alloc_opts, true, sizeof(hashmap_entry_t), true);  // TODO
        if (NULL == *dllptr)
        {
            return VPR_ERROR_HASHMAP_BUCKET_ALLOCATION_FAILED;
        }
    }

    // create the hash entry
    hashmap_entry_t* hmap_entry = (hashmap_entry_t*)allocate(
        hmap->options->alloc_opts, sizeof(hashmap_entry_t));
    if (NULL == hmap_entry)
    {
        release(hmap->options->alloc_opts, *dllptr);
        dllptr = NULL;
        return VPR_ERROR_HASHMAP_ENTRY_ALLOCATION_FAILED;
    }
    hmap_entry->key = key;
    hmap_entry->val = val;

    // add the entry to the linked list
    int dll_retval = doubly_linked_list_insert_end(*dllptr, hmap_entry);
    if (0 != dll_retval)
    {
        release(hmap->options->alloc_opts, hmap_entry);
        release(hmap->options->alloc_opts, *dllptr);
        dllptr = NULL;
        return dll_retval;
    }

    // success
    hmap->elements++;

    return VPR_STATUS_SUCCESS;
}


static doubly_linked_list_t* allocate_dll(
    allocator_options_t* alloc_opts, bool copy_on_insert,
    size_t element_size, bool release_on_dispose)
{
    // set up the options for this doubly linked list
    doubly_linked_list_options_t* dll_options =
        (doubly_linked_list_options_t*)allocate(
            alloc_opts, sizeof(doubly_linked_list_options_t));

    if (NULL == dll_options)
    {
        return NULL;
    }

    if (0 != doubly_linked_list_options_init(dll_options, alloc_opts, copy_on_insert, element_size, release_on_dispose))
    {
        release(alloc_opts, dll_options);
        return NULL;
    }

    // create and initialize the doubly linked list
    doubly_linked_list_t* dll = (doubly_linked_list_t*)allocate(
        alloc_opts, sizeof(doubly_linked_list_t));


    if (NULL == dll)
    {
        release(alloc_opts, dll_options);
        return NULL;
    }

    if (0 != doubly_linked_list_init(dll_options, dll))
    {
        release(alloc_opts, dll);
        release(alloc_opts, dll_options);
        return NULL;
    }

    return dll;
}
