#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>


int doubly_linked_list_insert_after(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element, void* data)
{

    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->first != NULL);
    MODEL_ASSERT(dll->last != NULL);
    MODEL_ASSERT(dll->elements > 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(element != NULL);
    MODEL_ASSERT(element->data != NULL);
    MODEL_ASSERT(data != NULL);

    /* create the element */
    doubly_linked_list_element_t* new_element =
        (doubly_linked_list_element_t*)allocate(
            dll->options->alloc_opts,
            sizeof(doubly_linked_list_element_t));

    /* allocate space for the data */
    uint8_t* data_buffer =
        (uint8_t*)allocate(
            dll->options->alloc_opts,
            dll->options->element_size);

    new_element->data = data_buffer;

    /* copy the data into the buffer */
    dll->options->doubly_linked_list_element_copy(
        dll->options->context,
        new_element->data, data, dll->options->element_size);


    /* set the pointers */
    new_element->prev = element;
    new_element->next = element->next;
    if (element->next == NULL)
    {
        dll->last = new_element;
    }
    else
    {
        element->next->prev = new_element;
    }
    element->next = new_element;


    /* increment the number of elements in this list*/
    dll->elements++;

    //success
    return VPR_STATUS_SUCCESS;
}
