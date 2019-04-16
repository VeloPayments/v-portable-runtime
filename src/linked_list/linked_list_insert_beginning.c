/**
 * \file linked_list_insert_beginning.c
 *
 * Implementation of linked_list_insert_beginning.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/linked_list.h>

/**
 * \brief Insert a new element at the beginning of a linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * at the beginning of the linked list.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          beginning of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero status code on failure.
 */
int linked_list_insert_beginning(linked_list_t* ll, void* data)
{
    MODEL_ASSERT(NULL != ll);
    MODEL_ASSERT(ll->elements >= 0);
    MODEL_ASSERT(NULL != ll->options);
    MODEL_ASSERT(NULL != ll->options->alloc_opts);
    MODEL_ASSERT(ll->options->element_size > 0);
    MODEL_ASSERT(NULL != data);

    // is this the first element in the list?
    if (NULL == ll->first)
    {
        return linked_list_insert_end(ll, data);
    }
    else
    {
        return linked_list_insert_before(ll, ll->first, data);
    }
}
