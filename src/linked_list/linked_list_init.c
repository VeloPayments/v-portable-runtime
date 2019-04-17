/**
 * \file linked_list_init.c
 *
 * Implementation of linked_list_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/linked_list.h>
#include <vpr/parameters.h>

//forward decls
static void ll_dispose(void*);

/**
 * \brief Initialize a linked list.
 *
 * This method allows for the creation of a linked list.  Once initialized, the
 * list will have zero elements, and the first and last pointers will be set to
 * null.
 *
 * When the function completes successfully, the caller owns this
 * ::linked_list_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The linked list options to use for this instance.
 * \param ll                The linked list to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int linked_list_init(linked_list_options_t* options, linked_list_t* ll)
{

    //the doubly linked list structure must be non-null
    MODEL_ASSERT(NULL != ll);

    //sanity checks on options
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(options->element_size > 0);
    MODEL_ASSERT(NULL != options->alloc_opts);

    ll->hdr.dispose = &ll_dispose;
    ll->options = options;
    ll->elements = 0;
    ll->first = NULL;
    ll->last = NULL;

    //success
    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of a linked list.
 *
 * \param pll        An opaque pointer to the linked list
 */
void ll_dispose(void* pll)
{
    MODEL_ASSERT(NULL != pll);
    linked_list_t* ll = (linked_list_t*)pll;
    MODEL_ASSERT(NULL != ll->options);
    MODEL_ASSERT(NULL != ll->options->alloc_opts);

    //dispose of each element in the list
    linked_list_element_t* element = ll->first;
    while (element != NULL)
    {

        // this call frees the memory for the data pointed to by the element
        if (NULL != element->data && NULL != ll->options->linked_list_element_dispose)
        {
            ll->options->linked_list_element_dispose(
                ll->options->alloc_opts, element->data);
        }

        // free the space for the element itself, being careful to
        // advance our pointer first
        linked_list_element_t* curr = element;
        element = element->next;

        release(ll->options->alloc_opts, curr);
    }
}
