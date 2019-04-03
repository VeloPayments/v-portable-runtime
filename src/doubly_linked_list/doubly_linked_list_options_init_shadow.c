/**
 * \file doubly_linked_list_options_init_shadow.c
 *
 * Shadow library implementation of doubly_linked_list_options_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/doubly_linked_list.h>
#include <vpr/parameters.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_dll_shadowed

/* forward decls for internal methods */
static void dll_simple_dispose(void*);

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

    options->hdr.dispose = &dll_simple_dispose;
    options->alloc_opts = alloc_opts;
    options->element_size = element_size;
    options->doubly_linked_list_element_copy = NULL;
    options->doubly_linked_list_element_dispose = NULL;

    return VPR_STATUS_SUCCESS;
}

/**
 * Dispose of the options structure.  Nothing special needs to be done.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void dll_simple_dispose(void* UNUSED(poptions))
{
    MODEL_ASSERT(NULL != poptions);
}


#endif /*defined(MODEL_CHECK_vpr_dll_shadowed)*/
