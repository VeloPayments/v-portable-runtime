/**
 * \file doubly_linked_list_insert_end.c
 *
 * Implementation of doubly_linked_list_insert_end.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/**
 * \brief Insert a new element at the end of a doubly linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * at the end of the linked list.
 *
 * WARNING: this function is not thread safe!
 *
 * \param dll               The doubly linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          end of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero error code on failure
 *
 */
int doubly_linked_list_insert_end(doubly_linked_list_t* dll, void* data)
{

    MODEL_ASSERT(NULL != dll);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(NULL != dll->options);
    MODEL_ASSERT(NULL != dll->options->alloc_opts);
    MODEL_ASSERT(NULL != data);

    /* is this the first element in the list? */
    if (NULL == dll->last)
    {
        return doubly_linked_list_insert_beginning(dll, data);
    }
    else
    {
        return doubly_linked_list_insert_after(dll, dll->last, data);
    }
}
