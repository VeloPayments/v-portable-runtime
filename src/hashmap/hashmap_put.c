/**
 * \file hashmap_put.c
 *
 * Implementation of hashmap_put.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

static doubly_linked_list_t* allocate_dll(
    allocator_options_t* alloc_opts, bool copy_on_insert,
    size_t element_size, bool release_on_dispose);

int hashmap_put(hashmap_t* hmap, void* UNUSED(data))
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != data);

    // figure out which bucket
    unsigned int bucket = 0;  // TODO

    // get the linked list to add this data to.  If there is not already a
    // linked list in the bucket, create one and add it to the bucket now.

    doubly_linked_list_t* dll = (doubly_linked_list_t*)hmap->buckets + bucket;
    if (NULL == dll)
    {
        dll = (doubly_linked_list_t*)allocate_dll(
            hmap->options->alloc_opts, false, sizeof(int), false);  // TODO
    }

    // add our data to the linked list
    // TODO

    hmap->elements++;

    return VPR_STATUS_SUCCESS;
}


static doubly_linked_list_t* allocate_dll(
    allocator_options_t* alloc_opts, bool copy_on_insert,
    size_t element_size, bool release_on_dispose)
{
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
