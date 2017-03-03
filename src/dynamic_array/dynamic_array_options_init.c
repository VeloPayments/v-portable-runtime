/**
 * \file dynamic_array_options_init.c
 *
 * Implementation of dynamic_array_options_init.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void darr_simple_elem_copy(void*, void*, const void*, size_t);
static void darr_simple_elem_dispose(void*, void*);

/**
 * Initialize dynamic array options for a POD data type.  This method should not
 * be used to initialize arrays with data types that cannot be copied directly
 * (e.g. with memcpy()).  An optional argument overrides the comparison method.
 * If this argument is set to NULL, then memcmp() will be used.  Please see
 * vpr/compare.h for a set of pre-defined comparison methods for most builtin C
 * types.
 *
 * \param options           The dynamic array options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param element_size      The size of an individual element.
 * \param compare_method    The comparison method to use for sorting, or NULL.
 *
 * \returns zero if successful, non-zero on failure.  On success, the caller
 * owns the dynamic array options structure and must call dispose() on the
 * structure to free any associated memory.
 */
int dynamic_array_options_init(
    dynamic_array_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size, compare_method_t compare_method)
{
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(alloc_opts != NULL);
    MODEL_ASSERT(alloc_opts->allocator_release != NULL);
    MODEL_ASSERT(element_size != 0);

    //set the compare method to memcmp if not set.
    if (compare_method == NULL)
        compare_method = &memcmp;

    //initialize this structure in terms of dynamic_array_options_init_ex
    return dynamic_array_options_init_ex(
        options, alloc_opts, element_size, options, &darr_simple_elem_copy,
        &darr_simple_elem_dispose, compare_method);
}

/**
 * The copy method to use when copying elements in this array.
 *
 * \param context       User-defined context for controlling the copy
 *                      method.
 * \param destination   The destination element to which this value will be
 *                      copied.
 * \param source        The source element used for the copy.
 * \param size          The size of the element being copied.
 */
static void darr_simple_elem_copy(
    void* UNUSED(context), void* destination, const void* source, size_t size)
{
    memcpy(destination, source, size);
}

/**
 * The dispose method to use when disposing an element in this array.
 *
 * \param context       User-defined context to use for the dispose method.
 * \param elem          The element to be disposed.
 */
static void darr_simple_elem_dispose(void* UNUSED(context), void* UNUSED(elem))
{
}
