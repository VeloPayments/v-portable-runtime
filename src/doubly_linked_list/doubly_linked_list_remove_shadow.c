/**
 * \file doubly_linked_list_remove_shadow.c
 *
 * Shadow library implementation of doubly_linked_list_remove.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_dll_shadowed

/**
 * \brief Remove an element from a doubly linked list.
 *
 * If successful, the specified element is removed from the linked list.  The
 * caller assumes responsibility for the element and the data referenced by the
 * element.
 *
 * WARNING: this function is not thread safe!
 *
 * \param dll               The doubly linked list.
 * \param element           The element to remove
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_remove(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element)
{
    MODEL_ASSERT(NULL != dll);
    MODEL_ASSERT(dll->elements > 0);
    MODEL_ASSERT(NULL != dll->options);
    MODEL_ASSERT(NULL != dll->options->alloc_opts);
    MODEL_ASSERT(NULL != element->data);
    MODEL_ASSERT(NULL != element->prev || NULL != element->next);

    --dll->elements;

    /* if this is the first element, just set the list first pointer to the
       next element */
    if (NULL == element->prev)
    {
        dll->first = element->next;
    }
    else
    {
        /* update the forward link in the previous element */
        element->prev->next = element->next;
    }

    /* if this is the last element, set the list last pointer to the
       previous element */
    if (NULL == element->next)
    {
        dll->last = element->prev;
    }
    else
    {
        /* update the reverse link in the next element */
        element->next->prev = element->prev;
    }

    //success
    return VPR_STATUS_SUCCESS;
}

#endif /*defined(MODEL_CHECK_vpr_dll_shadowed)*/
