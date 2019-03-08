#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>


int doubly_linked_list_insert_beginning(doubly_linked_list_t* dll, void* data)
{
    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(data != NULL);

    /* is this the first element in the list? */
    if (dll->first == NULL)
    {
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
        dll->first = new_element;
        dll->last = new_element;
        new_element->prev = NULL;
        new_element->next = NULL;

        /* the linked list now has a single element */
        dll->elements = 1UL;
    }
    else
    {
        doubly_linked_list_insert_before(dll, dll->first, data);
    }


    //success
    return VPR_STATUS_SUCCESS;
}
