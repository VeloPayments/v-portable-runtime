#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>


int doubly_linked_list_insert_beginning(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* new_element)
{
    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(new_element->data != NULL);

    /* is this the first element in the list? */
    if (dll->first == NULL)
    {
        dll->elements = 1UL;
        dll->first = new_element;
        dll->last = new_element;
        new_element->prev = NULL;
        new_element->next = NULL;
    }
    else
    {
        doubly_linked_list_insert_before(dll, dll->first, new_element);
    }


    //success
    return VPR_STATUS_SUCCESS;
}
