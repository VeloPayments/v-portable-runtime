#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/**
 * \brief Insert a new element after a specified element in a doubly linked
 * list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * after the specified element in the linked list.
 *
 * WARNING: this function is not thread safe!
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

    MODEL_ASSERT(NULL != dll);
    MODEL_ASSERT(NULL != dll->first);
    MODEL_ASSERT(NULL != dll->last);
    MODEL_ASSERT(dll->elements > 0);
    MODEL_ASSERT(NULL != dll->options);
    MODEL_ASSERT(NULL != dll->options->alloc_opts);
    MODEL_ASSERT(NULL != element);
    MODEL_ASSERT(NULL != element->data);
    MODEL_ASSERT(NULL != data);

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
    if (NULL == element->next)
    {
        dll->last = new_element;
    }
    else
    {
        element->next->prev = new_element;
    }
    element->next = new_element;

    /* increment the number of elements in this list*/
    ++dll->elements;

    /* if this is a copy-on-insert, then allocate space for the data
     * and copy it into that buffer.  Otherwise, set the pointer to
     * the original data. */
    if (NULL != dll->options->doubly_linked_list_element_copy)
    {
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
    }
    else
    {
        new_element->data = data;
    }

    //success
    return VPR_STATUS_SUCCESS;
}
