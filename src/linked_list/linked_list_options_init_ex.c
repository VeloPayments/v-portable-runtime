/**
 * \file linked_list_options_init_ex.c
 *
 * Implementation of linked_list_options_init_ex.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/linked_list.h>
#include <vpr/parameters.h>


/* forward decls for internal methods */
static void ll_simple_dispose(void*);

/**
 * \brief Initialize linked list options for a custom data type.
 *
 * This method allows the user to override the copy and dispose methods to
 * manage non-POD data types and complex data structures.
 *
 * When the function completes successfully, the caller owns this
 * ::linked_list_options_t instance and must dispose of it by calling
 * dispose() when it is no longer needed.
 *
 * \param options           The linked list options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param copy_method       Optional - The method to use to copy elements.
 *                          If provided then elements are copied into
 *                          separate memory as they are added to the list.
 * \param element_size      Optional (when copy_method is NULL).
 *                          The size of an individual element.
 * \param dispose_method    Optional - The method to use to dispose of data
 *                          within elements.
 *                          If provided then this method is invoked on each
 *                          element's data when the list is disposed of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int linked_list_options_init_ex(
    linked_list_options_t* options, allocator_options_t* alloc_opts,
    linked_list_element_copy_t copy_method, size_t element_size,
    linked_list_element_dispose_t dispose_method)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(NULL != alloc_opts->allocator_release);
    MODEL_ASSERT(NULL == copy_method || element_size > 0);

    options->hdr.dispose = &ll_simple_dispose;
    options->alloc_opts = alloc_opts;
    options->element_size = element_size;
    options->linked_list_element_copy = copy_method;
    options->linked_list_element_dispose = dispose_method;

    return VPR_STATUS_SUCCESS;
}

/**
 * Dispose of the options structure.  Nothing special needs to be done.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void ll_simple_dispose(void* UNUSED(poptions))
{
    MODEL_ASSERT(NULL != poptions);
}
