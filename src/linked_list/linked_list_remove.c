/**
 * \file linked_list_remove.c
 *
 * Implementation of linked_list_remove.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/linked_list.h>

/**
 * \brief Remove an element from a linked list.
 *
 * If successful, the specified element is removed from the linked list.  The
 * caller assumes responsibility for the element and the data referenced by the
 * element.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list.
 * \param element           The element to remove
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int linked_list_remove(linked_list_t* ll, linked_list_element_t* element)
{
    MODEL_ASSERT(NULL != ll);
    MODEL_ASSERT(ll->elements > 0);
    MODEL_ASSERT(NULL != ll->options);
    MODEL_ASSERT(NULL != ll->options->alloc_opts);
    MODEL_ASSERT(NULL != element->data);

    --ll->elements;

    // if this is the first element, just set the list first pointer to the
    // next element
    if (ll->first == element)
    {
        ll->first = element->next;

        // also the last element?
        if (ll->last == element)
        {
            ll->last = NULL;
        }
    }
    else
    {
        // find the previous element in the list and update the forward link
        linked_list_element_t* prev = ll->first;
        while (NULL != prev && prev->next != element)
        {
            prev = prev->next;
        }

        // if we didn't find it (the list doesn't contain the element being
        // removed), there is nothing to do.
        if (NULL == prev)
        {
            goto done;
        }

        // we found it.  update the forward link to skip the removed element.
        prev->next = element->next;

        // if we are removing the last element, fix the last pointer
        if (ll->last == element)
        {
            ll->last = prev;
        }
    }

done:
    //success
    return VPR_STATUS_SUCCESS;
}
