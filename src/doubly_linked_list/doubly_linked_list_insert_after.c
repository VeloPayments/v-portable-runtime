#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>


int doubly_linked_list_insert_after(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element,
    doubly_linked_list_element_t* new_element)
{

    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->first != NULL);
    MODEL_ASSERT(dll->last != NULL);
    MODEL_ASSERT(dll->elements > 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(element->data != NULL);
    MODEL_ASSERT(new_element->data != NULL);

    dll->elements++;

    new_element->prev = element;
    new_element->next = element->next;
    /* is this the new last element? */
    if (element->next == NULL)
    {
        dll->last = new_element;
    }
    else
    {
        element->next->prev = new_element;
    }
    element->next = new_element;


    //success
    return VPR_STATUS_SUCCESS;
}
