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
 * When the function completes successfully, the caller owns this
 * ::doubly_linked_list_options_t instance and must dispose of it by calling
 * dispose() when it is no longer needed.
 *
 * \param options           The dynamic array options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param copy_on_insert    If true, data will be copied before adding to the
 *                          linked list, leaving the caller with ownership of
 *                          the original data.  The linked list assumes
 *                          ownership of the copied data and will free it when
 *                          disposed of.
 *                          If false, the release_on_dispose argument dictates
 *                          what happens when the list is disposed of.
 * \param element_size      The size in bytes of an individual element.  This
 *                          parameter is ignored when copy_on_insert is false,
 *                          but must be a positive integer value if
 *                          copy_on_insert is true.
 * \param release_on_dispose  This parameter is ignored if copy_on_insert is
 *                          true.  When copy_on_insert is false, this argument
 *                          determines whether memory storing data encapsulated
 *                          within elements is released when the list is
 *                          disposed.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int doubly_linked_list_options_init(doubly_linked_list_options_t* options,
    allocator_options_t* alloc_opts, _Bool copy_on_insert,
    size_t element_size, _Bool release_on_dispose)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(NULL != alloc_opts->allocator_release);
    MODEL_ASSERT(0 != element_size);

    //initialize this structure in terms of doubly_linked_list_options_init_ex
    return doubly_linked_list_options_init_ex(options, alloc_opts,
        copy_on_insert ? &dll_simple_elem_copy : NULL,
        element_size,
        (copy_on_insert || release_on_dispose) ? &dll_simple_elem_dispose
                                               : NULL);
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
    MODEL_ASSERT(NULL != destination);
    MODEL_ASSERT(NULL != source);
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
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(NULL != elem);

    release(alloc_opts, elem);
}
