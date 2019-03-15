#include <stdlib.h>
#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/**
 * \brief Insert a new element at the beginning of a doubly linked list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed at the beginning of
 * this linked list.
 *
 * \param dll               The doubly linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          beginning of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero error code on failure
*/
int doubly_linked_list_insert_beginning(doubly_linked_list_t* dll, void* data)
{
    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(dll->options->element_size > 0);
    MODEL_ASSERT(data != NULL);

    /* is this the first element in the list? */
    if (dll->first == NULL)
    {
        /* create the element */
        doubly_linked_list_element_t* new_element =
            (doubly_linked_list_element_t*)allocate(
                dll->options->alloc_opts,
                sizeof(doubly_linked_list_element_t));
        if (NULL == new_element)
        {
            return VPR_ERROR_DLL_BEGINNING_ELEMENT_ALLOCATION_FAILED;
        }

        /* set the pointers */
        dll->first = new_element;
        dll->last = new_element;
        new_element->prev = NULL;
        new_element->next = NULL;

        /* the linked list now has a single element */
        dll->elements++;

        /* allocate space for the data */
        uint8_t* data_buffer = (uint8_t*)allocate(
            dll->options->alloc_opts,
            dll->options->element_size);
        new_element->data = data_buffer;
        if (NULL == data_buffer)
        {
            return VPR_ERROR_DLL_BEGINNING_BUFFER_ALLOCATION_FAILED;
        }

        /* copy the data into the buffer */
        dll->options->doubly_linked_list_element_copy(
            new_element->data, data, dll->options->element_size);

        //success
        return VPR_STATUS_SUCCESS;
    }
    else
    {
        return doubly_linked_list_insert_before(dll, dll->first, data);
    }
}
