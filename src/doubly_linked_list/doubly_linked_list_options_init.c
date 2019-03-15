/**
 * \file doubly_linked_list_options_init.c
 *
 * Implementation of doubly_linked_list_options_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void dll_simple_elem_copy(void*, const void*, size_t);
static void dll_simple_elem_dispose(allocator_options_t*, void*);

/**
 * \brief Initialize doubly linked list options for a POD data type.
 *
 * This method should not be used to initialize arrays with data types that
 * cannot be copied directly (e.g. with memcpy()).
 *
 * When the function completes successfully, the caller owns this
 * ::doubly_linked_list_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The dynamic array options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param element_size      The size of an individual element.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int doubly_linked_list_options_init(
    doubly_linked_list_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size)
{
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(alloc_opts != NULL);
    MODEL_ASSERT(alloc_opts->allocator_release != NULL);
    MODEL_ASSERT(element_size != 0);

    //initialize this structure in terms of doubly_linked_list_options_init_ex
    return doubly_linked_list_options_init_ex(
        options, alloc_opts, element_size,
        &dll_simple_elem_copy, &dll_simple_elem_dispose);
}

/**
 * The copy method to use when copying elements in this linked list.
 *
 * \param destination   The destination element to which this value will be
 *                      copied.
 * \param source        The source element used for the copy.
 * \param size          The size of the element being copied.
 */
static void dll_simple_elem_copy(void* destination, const void* source, size_t size)
{
    MODEL_ASSERT(destination != NULL);
    MODEL_ASSERT(source != NULL);
    MODEL_ASSERT(size > 0);

    memcpy(destination, source, size);
}

/**
 * The dispose method to use when disposing an element in this linked list.
 *
 * \param alloc_opts        The allocator options to use.
 * \param elem              The element to be disposed.
 */
static void dll_simple_elem_dispose(allocator_options_t* alloc_opts, void* elem)
{
    MODEL_ASSERT(alloc_opts != NULL);
    MODEL_ASSERT(elem != NULL);

    release(alloc_opts, elem);
}
