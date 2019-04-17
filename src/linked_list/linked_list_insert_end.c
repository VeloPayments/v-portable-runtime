/**
 * \file linked_list_insert_end.c
 *
 * Implementation of linked_list_insert_end.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/linked_list.h>

/**
 * \brief Insert a new element at the end of a linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * at the end of the linked list.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          end of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero status code on failure.
 */
int linked_list_insert_end(linked_list_t* ll, void* data)
{
    MODEL_ASSERT(NULL != ll);
    MODEL_ASSERT(ll->elements >= 0);
    MODEL_ASSERT(NULL != ll->options);
    MODEL_ASSERT(NULL != ll->options->alloc_opts);
    MODEL_ASSERT(NULL != data);

    // is this the first element in the list?
    if (NULL == ll->last)
    {
        // create the element
        linked_list_element_t* new_element =
            (linked_list_element_t*)allocate(
                ll->options->alloc_opts,
                sizeof(linked_list_element_t));
        if (NULL == new_element)
        {
            return VPR_ERROR_LL_END_ELEMENT_ALLOCATION_FAILED;
        }

        // set the pointers
        ll->first = new_element;
        ll->last = new_element;
        new_element->next = NULL;

        // the linked list now has a single element
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
                return VPR_ERROR_LL_END_BUFFER_ALLOCATION_FAILED;
            }

            // copy the data into the buffer
            ll->options->linked_list_element_copy(
                new_element->data, data, ll->options->element_size);
        }
        else
        {
            new_element->data = data;
        }

        //success
        return VPR_STATUS_SUCCESS;
    }
    else
    {
        return linked_list_insert_after(ll, ll->last, data);
    }
}
