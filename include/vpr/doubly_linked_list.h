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
* \brief The copy method to use when copying elements in this linked list.
*
* \param context       User-defined context for controlling the copy
*                      method.
* \param destination   The destination element to which this value will be
*                      copied.
* \param source        The source element used for the copy.
* \param size          The size of the element being copied.
*/
typedef void (*doubly_linked_list_element_copy_t)(
    void* context, void* destination, const void* source, size_t size);

/**
 * \brief The dispose method to use when disposing an element in this linked
 * list.
 *
 * \param context       User-defined context to use for the dispose method.
 * \param elem          The element to be disposed.
 */
typedef void (*doubly_linked_list_element_dispose_t)(
    void* context, void* elem);

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
     * \brief The element size of lists produced with this set of options.
     */
    size_t element_size;

    /**
     * \brief The copy method to use when copying elements in this linked list.
     *
     * \param context       User-defined context for controlling the copy
     *                      method.
     * \param destination   The destination element to which this value will be
     *                      copied.
     * \param source        The source element used for the copy.
     * \param size          The size of the element being copied.
     */
    doubly_linked_list_element_copy_t doubly_linked_list_element_copy;

    /**
     * \brief The dispose method to use when disposing an element in this
     * linked list.
     *
     * \param context       User-defined context to use for the dispose method.
     * \param elem          The element to be disposed.
     */
    doubly_linked_list_element_dispose_t doubly_linked_list_element_dispose;

    /**
     * \brief Context passed to user-defined methods.
     */
    void* context;

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
    doubly_linked_list_options_t* options,
    allocator_options_t* alloc_opts,
    size_t element_size);

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
 * \param context           The context value to pass to the copy and dispose
 *                          methods.
 * \param copy_method       The method to use to copy elements.
 * \param dispose_method    The method to use to dispose elements.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int doubly_linked_list_options_init_ex(
    doubly_linked_list_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size, void* context,
    doubly_linked_list_element_copy_t copy_method,
    doubly_linked_list_element_dispose_t dispose_method);

/**
 * \brief Initialize a doubly linked list.
 *
 * This method allows for the creation of a doubly linked list.  Once initialized,
 * the list will have zero elements, and the first and last pointers will
 * be set to null.
 *
 * When the function completes successfully, the caller owns this
 * ::doubly_linked_list_t instance and must dispose of it by calling dispose() when
 * it is no longer needed.
 *
 * \param options           The doubly linked list options to use for this instance.
 * \param dll               The doubly linked list to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_init(
    doubly_linked_list_options_t* options, doubly_linked_list_t* dll);

/**
 * \brief Insert a new element at the beginning of a doubly linked list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed at the beginning of
 * this linked list.
 *
 * \param dll               The doubly linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          beginning of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_insert_beginning(doubly_linked_list_t* dll, void* data);

/**
 * \brief Insert a new element at the end of a doubly linked list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed at the end of this
 * linked list.
 *
 * \param dll               The doubly linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          end of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_insert_end(doubly_linked_list_t* dll, void* data);

/**
 * \brief Insert a new element before a specified element in a doubly linked
 * list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed before the specified
 * element in the linked list.
 *
 * \param dll               The doubly linked list
 * \param element           The existing element, which will succeed the
 *                          new element.
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted in the
 *                          list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_insert_before(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element, void* data);

/**
 * \brief Insert a new element after a specified element in a doubly linked
 * list.
 *
 * If successful, then a copy of this data will be made using the defined
 * copy method, then encapsulated in an element and placed after the specified
 * element in the linked list.
 *
 * \param dll               The doubly linked list
 * \param element           The existing element, which will precede the
 *                          new element.
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted in the
 *                          list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_insert_after(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element, void* data);

/**
 * \brief Remove an element from a doubly linked list.
 *
 * If successful, the specified element is removed from the linked list.  The
 * caller assumes responsibility for the element and the data referenced by the
 * element.
 *
 * \param dll               The doubly linked list.
 * \param element           The element to remove
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int doubly_linked_list_remove(doubly_linked_list_t* dll,
    doubly_linked_list_element_t* element);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_DOUBLY_LINKED_LIST_HEADER_GUARD
