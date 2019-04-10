/**
 * \file hashmap_init.c
 *
 * Implementation of hashmap_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/hashmap.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

//forward decls
static void hashmap_dispose(void*);
static void dispose_hashmap_items_in_dll(doubly_linked_list_t*,
    hashmap_options_t*);

/**
 * \brief Initialize a hashmap.
 *
 * This method allows for the creation of a hashmap.  Once initialized the
 * hashmap will be empty.

 * When the function completes successfully, the caller owns this
 * ::hashmap_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The hashmap options to use for this instance.
 * \param hmap              The hashmap to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_HASHMAP_ALLOCATION_FAILED if memory could not
 *        be allocated for the hashmap
 */
int hashmap_init(hashmap_options_t* options, hashmap_t* hmap)
{
    //the hashmap structure must be non-null
    MODEL_ASSERT(NULL != hmap);

    //sanity checks on options
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != options->alloc_opts);

    hmap->hdr.dispose = &hashmap_dispose;
    hmap->options = options;

    // allocate the space for the hashmap
    hmap->buckets = (void*)allocate(
        hmap->options->alloc_opts,
        hmap->options->capacity * sizeof(uint8_t*));
    if (NULL == hmap->buckets)
    {
        return VPR_ERROR_HASHMAP_ALLOCATION_FAILED;
    }

    // clear the hashmap
    void** buckets = hmap->buckets;
    for (uint32_t i = 0; i < hmap->options->capacity; i++)
    {
        buckets[i] = NULL;
    }

    // this hashmap has no elements
    hmap->elements = 0;

    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of a hashmap.
 *
 * \param phmap        An opaque pointer to the hashmap.
 */
void hashmap_dispose(void* phmap)
{
    MODEL_ASSERT(NULL != phmap);

    hashmap_t* hmap = (hashmap_t*)phmap;

    MODEL_ASSERT(NULL != hmap->options);
    MODEL_ASSERT(NULL != hmap->options->alloc_opts);

    // dispose of the linked lists within the buckets
    void** buckets = hmap->buckets;
    for (uint32_t i = 0; i < hmap->options->capacity; i++)
    {
        doubly_linked_list_t* dll = (doubly_linked_list_t*)buckets[i];
        if (NULL != dll)
        {
            doubly_linked_list_options_t* options = dll->options;

            // are we responsible for the data items in each hashmap entry?
            if (NULL != hmap->options->hashmap_item_dispose)
            {
                dispose_hashmap_items_in_dll(dll, hmap->options);
            }

            // dispose of the DLL, which will also release the hashmap entries
            dispose((disposable_t*)dll);

            // release memory for DLL and options
            release(hmap->options->alloc_opts, dll);
            release(hmap->options->alloc_opts, options);
        }
    }

    release(hmap->options->alloc_opts, hmap->buckets);
}

/**
 * Dispose of all the data contained in hashmap entries in a DLL
 *
 * \param dll           The doubly linked list containing hashmap entries
 * \param hmap_options  The options for the hashmap
 */
static void dispose_hashmap_items_in_dll(
    doubly_linked_list_t* dll, hashmap_options_t* hmap_options)
{

    MODEL_ASSERT(NULL != dll);
    MODEL_ASSERT(NULL != hmap_options);
    MODEL_ASSERT(NULL != hmap_options->alloc_opts);
    MODEL_ASSERT(NULL != hmap_options->hashmap_item_dispose);

    doubly_linked_list_element_t* element = dll->first;
    while (element != NULL)
    {
        hashmap_entry_t* hmap_entry = (hashmap_entry_t*)element->data;
        hmap_options->hashmap_item_dispose(
            hmap_options->alloc_opts, hmap_entry->val);
        element = element->next;
    }
}
