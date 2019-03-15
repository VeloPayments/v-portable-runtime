#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/**
 * \brief Insert a new element after a specified element in a doubly linked
 * list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed after the specified
 * element in the linked list.
 *
 * \param dll               The doubly linked list
 * \param element           The existing element, which will precede the
 *                          new element.
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted in the
 *                          list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - \ref VPR_ERROR_DLL_AFTER_ELEMENT_ALLOCATION_FAILED if
 *              memory could not be allocated for a new element
 *          - \ref VPR_ERROR_DLL_AFTER_BUFFER_ALLOCATION_FAILED if
 *              memory could not be allocated to copy the data
 */
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
    if (NULL == new_element)
    {
        return VPR_ERROR_DLL_AFTER_ELEMENT_ALLOCATION_FAILED;
    }

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

    /* allocate space for the data */
    uint8_t* data_buffer = (uint8_t*)allocate(
        dll->options->alloc_opts,
        dll->options->element_size);
    new_element->data = data_buffer;
    if (NULL == data_buffer)
    {
        return VPR_ERROR_DLL_AFTER_BUFFER_ALLOCATION_FAILED;
    }

    /* copy the data into the buffer */
    dll->options->doubly_linked_list_element_copy(
        new_element->data, data, dll->options->element_size);


    //success
    return VPR_STATUS_SUCCESS;
}
