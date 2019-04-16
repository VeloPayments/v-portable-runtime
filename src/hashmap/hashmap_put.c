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
 * \brief Add a value to a hashmap.
 *
 * Add a value to the hashmap using a variable length key.
 *
 * \param hmap              The hashmap to add the value to.
 * \param key               A unique key that serves as an identifier for the
 *                          value.
 * \param key_len           The length of the key.
 * \param val               Opaque pointer to the value.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - non-zero code on failure.
 */
int hashmap_put(hashmap_t* hmap, uint8_t* key, size_t key_len, void* val)
{
    MODEL_ASSERT(NULL != hmap);
    MODEL_ASSERT(NULL != hmap->options);
    MODEL_ASSERT(hmap->options->capacity > 0);
    MODEL_ASSERT(NULL != key);
    MODEL_ASSERT(key_len > 0);
    MODEL_ASSERT(NULL != val);

    // figure out which bucket
    uint64_t hashed_key = hmap->options->hash_func(key, key_len);
    uint32_t bucket = hashed_key % hmap->options->capacity;

    // get the linked list in that bucket.  If there is not already one,
    // create one and add it to the bucket now.
    void** buckets = hmap->buckets;
    doubly_linked_list_t* dllptr = (doubly_linked_list_t*)buckets[bucket];

    if (NULL == dllptr)
    {
        dllptr = (doubly_linked_list_t*)
            allocate_dll(hmap->options->alloc_opts);
        if (NULL == dllptr)
        {
            return VPR_ERROR_HASHMAP_BUCKET_ALLOCATION_FAILED;
        }
        buckets[bucket] = dllptr;
    }

    // create the hash entry
    hashmap_entry_t* hmap_entry = (hashmap_entry_t*)allocate(
        hmap->options->alloc_opts, sizeof(hashmap_entry_t));
    if (NULL == hmap_entry)
    {
        return VPR_ERROR_HASHMAP_ENTRY_ALLOCATION_FAILED;
    }

    hmap_entry->hashed_key = hashed_key;

    // if this is a copy-on-insert, then allocate space for the data and copy
    // it into that buffer.  Otherwise, set the pointer to the original data.
    if (NULL != hmap->options->copy_method)
    {
        uint8_t* data_buffer = (uint8_t*)allocate(
            hmap->options->alloc_opts,
            hmap->options->val_size);
        hmap_entry->val = data_buffer;
        if (NULL == data_buffer)
        {
            release(hmap->options->alloc_opts, hmap_entry);
            return VPR_ERROR_HASHMAP_DATA_ITEM_ALLOCATION_FAILED;
        }

        // copy the data into the buffer
        hmap->options->copy_method(
            hmap_entry->val, val, hmap->options->val_size);
    }
    else
    {
        // not copying, just reference data passed in.
        hmap_entry->val = val;
    }

    // add the entry to the linked list
    int dll_retval = doubly_linked_list_insert_beginning(dllptr, hmap_entry);
    if (0 != dll_retval)
    {
        if (NULL != hmap->options->copy_method)
        {
            release(hmap->options->alloc_opts, hmap_entry->val);
        }
        release(hmap->options->alloc_opts, hmap_entry);
        return dll_retval;
    }

    // success
    hmap->elements++;

    // scan the rest of the LL looking for values with the same key
    // remove any that are found.
    doubly_linked_list_element_t* element = dllptr->first->next;
    while (NULL != element)
    {
        doubly_linked_list_element_t* next_element = element->next;
        hashmap_entry_t* hmap_entry = (hashmap_entry_t*)element->data;
        if (hmap_entry->hashed_key == hashed_key)
        {
            // the hashed keys match, which almost guarantees a match.  If an
            // equality function was supplied, use it as a verification.
            if (NULL == hmap->options->equals_func || hmap->options->equals_func(key, hmap_entry->val))
            {
                remove_hashmap_entry(hmap, hmap_entry, dllptr, element);
            }
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
    MODEL_ASSERT(hmap->elements > 0);

    doubly_linked_list_remove(dll, element);

    // this call frees the memory for the data, if appropriate
    if (NULL != hmap_entry->val && NULL != hmap->options->dispose_method)
    {
        hmap->options->dispose_method(
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

    // the elements in this DLL are hash_entries.  We don't want to copy them;
    // the DLL will assume ownership of the hash entries added to them.
    // However, we do need to release the hash entries when we dispose of
    // the DLL.
    if (0 != doubly_linked_list_options_init(dll_options, alloc_opts, false, sizeof(hashmap_entry_t), true))
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
