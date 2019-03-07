/**
 * \file doubly_linked_list.h
 *
 * \brief Doubly linked list
 *
 * The doubly linked list provides forward and backward traversal of a linked
 * list.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_DOUBLY_LINKED_LIST_HEADER_GUARD
#define VPR_DOUBLY_LINKED_LIST_HEADER_GUARD

#include <vpr/allocator.h>
#include <vpr/compare.h>
#include <vpr/disposable.h>
#include <vpr/error_codes.h>
#include <stdbool.h>
#include <stdlib.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \brief This structure contains the options used by a doubly linked list
 * instance.
 *
 * User code will create an options structure using the
 * doubly_linked_list_options_init() method declared below.  When this set of
 * options is no longer required, it should be disposed using dispose().
 */
typedef struct doubly_linked_list_options
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The allocator options to use when creating doubly linked lists.
     */
    allocator_options_t* alloc_opts;

    /**
     * \brief Context passed to user-defined methods.
     */
    //   void* context;  // TODO: do we need this?

} doubly_linked_list_options_t;

/**
 * \brief An element of the doubly linked list.
 */
typedef struct doubly_linked_list_element
{

    /**
     * \brief Pointer to the previous element in the list, or
     * NULL if this is the first element.
     */
    struct doubly_linked_list_element* prev;

    /**
     * \brief Pointer to the next element in the list, or NULL
     * if this is the last element.
     */
    struct doubly_linked_list_element* next;

    /**
     * \brief Opaque pointer to the data stored in this element.
     */
    void* data;

} doubly_linked_list_element_t;

/**
 * \brief The doubly linked list structure.
 *
 */
typedef struct doubly_linked_list
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The options used to create this list.
     */
    doubly_linked_list_options_t* options;

    /**
     * \brief The number of elements currently instantiated.
     */
    size_t elements;

    /**
     * \brief Pointer to the first element in the list.
     */
    doubly_linked_list_element_t* first;

    /**
     * \brief Pointer to the last element in the list.
     */
    doubly_linked_list_element_t* last;

} doubly_linked_list_t;


/**
 * \brief Initialize doubly linked list options.
 *
 * When the function completes successfully, the caller owns this
 * ::doubly_linked_list_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The doubly linked list options to initialize.
 * \param alloc_opts        The allocator options to use.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int doubly_linked_list_options_init(
    doubly_linked_list_options_t* options,
    allocator_options_t* alloc_opts);

int doubly_linked_list_init(
    doubly_linked_list_options_t* options, doubly_linked_list_t* dll);

int doubly_linked_list_insert_beginning(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* first_element);

int doubly_linked_list_insert_end(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* first_element);

int doubly_linked_list_insert_before(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element,
    doubly_linked_list_element_t* new_element);

int doubly_linked_list_insert_after(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element,
    doubly_linked_list_element_t* new_element);

int doubly_linked_list_remove(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_DOUBLY_LINKED_LIST_HEADER_GUARD
