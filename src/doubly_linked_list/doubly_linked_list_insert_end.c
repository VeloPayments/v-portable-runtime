#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

int doubly_linked_list_insert_end(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* last_element)
{

    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(new_element->data != NULL);


    /* is this the first element in the list? */
    if (dll->last == NULL)
    {
        doubly_linked_list_insert_beginning(dll, last_element);
    }
    else
    {
        doubly_linked_list_insert_after(dll, dll->last, last_element);
    }

    //success
    return VPR_STATUS_SUCCESS;
}
