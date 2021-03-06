/**
 * \file linked_list_insert_before.c
 *
 * Implementation of linked_list_insert_before
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/linked_list.h>


/**
 * \brief Insert a new element before a specified element in a linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * before the specified element in the linked list.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list
 * \param element           The existing element, which will succeed the
 *                          new element.
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted in the
 *                          list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero status code on failure.
 */
int linked_list_insert_before(
    linked_list_t* ll, linked_list_element_t* element, void* data)
{
    MODEL_ASSERT(NULL != ll);
    MODEL_ASSERT(NULL != ll->first);
    MODEL_ASSERT(NULL != ll->last);
    MODEL_ASSERT(ll->elements > 0);
    MODEL_ASSERT(NULL != ll->options);
    MODEL_ASSERT(NULL != ll->options->alloc_opts);
    MODEL_ASSERT(NULL != element);
    MODEL_ASSERT(NULL != element->data);
    MODEL_ASSERT(NULL != data);

    // find the previous element so we can update the forward link
    linked_list_element_t* prev = NULL;
    if (ll->first != element)
    {
        prev = ll->first;
        while (NULL != prev && prev->next != element)
        {
            prev = prev->next;
        }

        // if we didn't find it then don't do anything
        if (NULL == prev)
        {
            goto done;
        }
    }

    // create the element
    linked_list_element_t* new_element =
        (linked_list_element_t*)allocate(
            ll->options->alloc_opts,
            sizeof(linked_list_element_t));
    if (NULL == new_element)
    {
        return VPR_ERROR_LL_BEFORE_ELEMENT_ALLOCATION_FAILED;
    }

    // set the pointers
    if (NULL == prev)  // would only be null if this is the new first
    {
        ll->first = new_element;
    }
    else
    {
        prev->next = new_element;
    }
    new_element->next = element;


    // increment the number of elements in this list
    ++ll->elements;

    // if this is a copy-on-insert, then allocate space for the data
    // and copy it into that buffer.  Otherwise, set the pointer to
    // the original data.
    if (NULL != ll->options->linked_list_element_copy)
    {
        uint8_t* data_buffer = (uint8_t*)allocate(
            ll->options->alloc_opts,
            ll->options->element_size);
        new_element->data = data_buffer;
        if (NULL == data_buffer)
        {
            return VPR_ERROR_LL_BEFORE_BUFFER_ALLOCATION_FAILED;
        }

        // copy the data into the buffer
        ll->options->linked_list_element_copy(
            new_element->data, data, ll->options->element_size);
    }
    else
    {
        new_element->data = data;
    }

done:
    //success
    return VPR_STATUS_SUCCESS;
}
