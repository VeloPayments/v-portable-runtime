/**
 * \file dynamic_array_options_init_ex.c
 *
 * Implementation of dynamic_array_options_init_ex.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void darr_simple_dispose(void*);

/**
 * Initialize dynamic array options for a custom data type.  This method allows
 * the user to override the copy and dispose methods to manage non-POD data
 * types and complex data structures.
 *
 * \param options           The dynamic array options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param element_size      The size of an individual element.
 * \param context           The context value to pass to the copy and dispose
 *                          methods.
 * \param copy_method       The method to use to copy elements.
 * \param dispose_method    The method to use to dispose elements.
 * \param compare_method    The comparison method to use for sorting, or NULL.
 *
 * \returns zero if successful, non-zero on failure.  On success, the caller
 * owns the dynamic array options structure and must call dispose() on the
 * structure to free any associated memory.
 */
int dynamic_array_options_init_ex(
    dynamic_array_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size, void* context,
    dynamic_array_element_copy_t copy_method,
    dynamic_array_element_dispose_t dispose_method,
    compare_method_t compare_method)
{
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(alloc_opts != NULL);
    MODEL_ASSERT(alloc_opts->allocator_release != NULL);
    MODEL_ASSERT(element_size != 0);
    MODEL_ASSERT(copy_method != 0);
    MODEL_ASSERT(dispose_method != 0);
    MODEL_ASSERT(compare_method != 0);

    //clear the options structure for future compatibility.
    memset(options, 0, sizeof(dynamic_array_options_t));

    //use our dispose method to dispose of these options
    options->hdr.dispose = &darr_simple_dispose;
    //use the user-supplied allocator options
    options->alloc_opts = alloc_opts;
    //set the element size
    options->element_size = element_size;
    //use our internal element copy method
    options->dynamic_array_element_copy = copy_method;
    //set the dispose method
    options->dynamic_array_element_dispose = dispose_method;
    //set the comparison method
    options->dynamic_array_element_compare = compare_method;
    //set the context
    options->context = context;

    return 0;
}

/**
 * Dispose of the options structure.  This method just clears the structure.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void darr_simple_dispose(void* poptions)
{
    MODEL_ASSERT(poptions != NULL);

    memset(poptions, 0, sizeof(dynamic_array_options_t));
}
