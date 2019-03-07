/**
 * \file doubly_linked_list_init.c
 *
 * Implementation of doubly_linked_list_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

//forward decls
static void dll_dispose(void*);

int doubly_linked_list_init(
    doubly_linked_list_options_t* options, doubly_linked_list_t* dll)
{

    //the doubly linked list structure must be non-null
    MODEL_ASSERT(dll != NULL);

    //sanity checks on options
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(options->alloc_opts != NULL);

    dll->hdr.dispose = &dll_dispose;
    dll->options = options;
    dll->elements = 0;
    dll->first = 0;
    dll->last = 0;

    //success
    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of a doubly linked list.
 *
 * \param pdll        An opaque pointer to the doubly linked list
 */
void dll_dispose(void* UNUSED(pdll))
{
    //    MODEL_ASSERT(pdll != NULL);
    //    doubly_linked_list_t* dll = (doubly_linked_list_t*)pdll;
    //    MODEL_ASSERT(dll->options != NULL);
    //    MODEL_ASSERT(dll->options->alloc_opts != NULL);
    //    MODEL_ASSERT(array->options->dynamic_array_element_dispose != NULL);
    //    MODEL_ASSERT(array->array != NULL);
    //    MODEL_ASSERT(array->elements <= array->reserved_elements);

    //dispose of each element in the array
    /*uint8_t* barr = (uint8_t*)array->array;
    for (size_t i = 0; i < array->elements; ++i)
    {
        array->options->dynamic_array_element_dispose(
                array->options->context, barr + i*array->options->element_size);
    }*/

    //release the memory
    //release(dll->options->alloc_opts, array->array);
}
