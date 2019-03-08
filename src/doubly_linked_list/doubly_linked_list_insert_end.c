#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/**
 * \brief Insert a new element at the end of a doubly linked list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed at the end of this
 * linked list.
 *
 * \param dll               The doubly linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          end of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_insert_end(doubly_linked_list_t* dll, void* data)
{

    MODEL_ASSERT(dll != NULL);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(dll->options != NULL);
    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    MODEL_ASSERT(data != NULL);

    /* is this the first element in the list? */
    if (dll->last == NULL)
    {
        doubly_linked_list_insert_beginning(dll, data);
    }
    else
    {
        doubly_linked_list_insert_after(dll, dll->last, data);
    }

    //success
    return VPR_STATUS_SUCCESS;
}
