/**
 * \file doubly_linked_list_options_init_ex.c
 *
 * Implementation of doubly_linked_list_options_init_ex.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>


/* forward decls for internal methods */
static void dll_simple_dispose(void*);

/**
 * \brief Initialize doubly linked list options for a custom data type.
 *
 * This method allows the user to override the copy and dispose methods to
 * manage non-POD data types and complex data structures.
 *
 * When the function completes successfully, the caller owns this
 * ::doubly_linked_list_t instance and must dispose of it by calling dispose() when
 * it is no longer needed.
 *
 * \param options           The doubly linked list options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param element_size      The size of an individual element.
 * \param copy_method       The method to use to copy elements.
 * \param dispose_method    The method to use to dispose elements.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int doubly_linked_list_options_init_ex(
    doubly_linked_list_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size, doubly_linked_list_element_copy_t copy_method,
    doubly_linked_list_element_dispose_t dispose_method)
{
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(alloc_opts != NULL);
    MODEL_ASSERT(alloc_opts->allocator_release != NULL);
    MODEL_ASSERT(element_size != 0);
    MODEL_ASSERT(copy_method != NULL);
    MODEL_ASSERT(dispose_method != NULL);

    //use our dispose method to dispose of these options
    options->hdr.dispose = &dll_simple_dispose;
    //use the user-supplied allocator options
    options->alloc_opts = alloc_opts;
    //set the element size
    options->element_size = element_size;
    //use our internal element copy method
    options->doubly_linked_list_element_copy = copy_method;
    //set the dispose method
    options->doubly_linked_list_element_dispose = dispose_method;

    return VPR_STATUS_SUCCESS;
}

/**
 * Dispose of the options structure.  Nothing special needs to be done.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void dll_simple_dispose(void* UNUSED(poptions))
{
    MODEL_ASSERT(poptions != NULL);
}
