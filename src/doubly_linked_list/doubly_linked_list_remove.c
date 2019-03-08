#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/**
 * \brief Remove an element from a doubly linked list.
 *
 * If successful, the specified element is removed from the linked list.  The
 * caller assumes responsibility for the element and the data referenced by the
 * element.
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
    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements > 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(element->data != NULL);
    MODEL_ASSERT(element->prev != NULL || element->next != NULL);

    dll->elements--;

    /* if this is the first element, just set the list first pointer to the
       next element */
    if (element->prev == NULL)
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
    if (element->next == NULL)
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
