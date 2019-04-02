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

static void remove_hashmap_entry(hashmap_t*, hashmap_entry_t*,
    doubly_linked_list_t*, doubly_linked_list_element_t*);
static doubly_linked_list_t* allocate_dll(allocator_options_t*);

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

    // figure out which bucket
    unsigned int bucket = key % hmap->options->capacity;

    // get the linked list in that bucket.  If there is not already one,
    // create one and add it to the bucket now.
    doubly_linked_list_t** dllptr =
        bucket + (doubly_linked_list_t**)hmap->buckets;
    if (NULL == *dllptr)
    {
        *dllptr = (doubly_linked_list_t*)allocate_dll(hmap->options->alloc_opts);
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

    // if this is a copy-on-insert, then allocate space for the data and copy
    // it into that buffer.  Otherwise, set the pointer to the original data.
    if (NULL != hmap->options->hashmap_item_copy)
    {
        uint8_t* data_buffer = (uint8_t*)allocate(
            hmap->options->alloc_opts,
            hmap->options->item_size);
        hmap_entry->val = data_buffer;
        if (NULL == data_buffer)
        {
            return VPR_ERROR_HASHMAP_DATA_ITEM_ALLOCATION_FAILED;
        }

        // copy the data into the buffer
        hmap->options->hashmap_item_copy(
            hmap_entry->val, val, hmap->options->item_size);
    }
    else
    {
        // not copying, just reference data passed in.
        hmap_entry->val = val;
    }

    // add the entry to the linked list
    int dll_retval = doubly_linked_list_insert_beginning(*dllptr, hmap_entry);
    if (0 != dll_retval)
    {
        release(hmap->options->alloc_opts, hmap_entry);
        release(hmap->options->alloc_opts, *dllptr);
        dllptr = NULL;
        return dll_retval;
    }

    // success
    hmap->elements++;

    // scan the rest of the LL looking for values with the same key
    // remove any that are found.
    doubly_linked_list_element_t* element = (*dllptr)->first->next;
    while (element != NULL)
    {
        doubly_linked_list_element_t* next_element = element->next;
        hashmap_entry_t* hmap_entry = (hashmap_entry_t*)element->data;
        if (hmap_entry->key == key)
        {
            remove_hashmap_entry(hmap, hmap_entry, *dllptr, element);
        }

        element = next_element;
    }


    return VPR_STATUS_SUCCESS;
}

/**
 * Remove a hashmap entry from the doubly linked list it is contained in.
 *
 * \param hmap                  The hashmap
 * \param hmap_entry            The hashmap entry to remove.
 * \param dll                   The doubly linked list containing the entry.
 * \param element               The DLL element encapsulating the hashmap
 *                              entry.
 */
static void remove_hashmap_entry(hashmap_t* hmap, hashmap_entry_t* hmap_entry,
    doubly_linked_list_t* dll, doubly_linked_list_element_t* element)
{
    doubly_linked_list_remove(dll, element);

    // this call frees the memory for the data, if appropriate
    if (NULL != hmap_entry->val && NULL != hmap->options->hashmap_item_dispose)
    {
        hmap->options->hashmap_item_dispose(
            hmap->options->alloc_opts, hmap_entry->val);
    }

    // free the hashmap entry
    release(hmap->options->alloc_opts, hmap_entry);

    // free the memory for the DLL element
    release(dll->options->alloc_opts, element);

    hmap->elements--;
}

/**
 * Create a doubly linked list.
 *
 * The LL will be configured to assume ownership (not copy) the hash entries
 * that are added to it, and to release the memory allocated for each
 * hash entry when the list is disposed of.
 *
 * \param alloc_opts            The allocator options to use for this LL
 * \return a pointer to the DLL, or NULL on failure
 */
static doubly_linked_list_t* allocate_dll(allocator_options_t* alloc_opts)
{
    // set up the options for this doubly linked list
    doubly_linked_list_options_t* dll_options =
        (doubly_linked_list_options_t*)allocate(
            alloc_opts, sizeof(doubly_linked_list_options_t));

    if (NULL == dll_options)
    {
        return NULL;
    }

    int dll_options_init_retval = 0;
    MODEL_EXEMPT(dll_options_init_retval = doubly_linked_list_options_init(
                     dll_options, alloc_opts, false, sizeof(hashmap_entry_t), true));

    if (0 != dll_options_init_retval)
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

    int dll_init_retval = 0;
    MODEL_EXEMPT(dll_init_retval = doubly_linked_list_init(dll_options, dll));
    if (0 != dll_init_retval)
    {
        release(alloc_opts, dll);
        release(alloc_opts, dll_options);
        return NULL;
    }

    return dll;
}
