/**
 * \file doubly_linked_list_insert_beginning_shadow.c
 *
 * Shadow library implementation of doubly_linked_list_insert_beginning.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_dll_shadowed

/**
 * \brief Insert a new element at the beginning of a doubly linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * at the beginning of the linked list.
 *
 * WARNING: this function is not thread safe!
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
    MODEL_ASSERT(NULL != dll);
    MODEL_ASSERT(dll->elements >= 0);
    MODEL_ASSERT(NULL != dll->options);
    MODEL_ASSERT(NULL != dll->options->alloc_opts);
    MODEL_ASSERT(dll->options->element_size > 0);
    MODEL_ASSERT(NULL != data);

    /* create the element */
    doubly_linked_list_element_t* new_element =
        (doubly_linked_list_element_t*)allocate(
            dll->options->alloc_opts,
            sizeof(doubly_linked_list_element_t));
    if (NULL == new_element)
    {
        return VPR_ERROR_DLL_BEGINNING_ELEMENT_ALLOCATION_FAILED;
    }

    // we only allow one element in the shadow impl
    MODEL_ASSERT(NULL == dll->first);

    /* set the pointers */
    dll->first = new_element;
    dll->last = new_element;
    new_element->prev = NULL;
    new_element->next = NULL;

    /* the linked list now has a single element */
    dll->elements = 1;

    new_element->data = data;

    //success
    return VPR_STATUS_SUCCESS;
}

#endif /*defined(MODEL_CHECK_vpr_dll_shadowed)*/
