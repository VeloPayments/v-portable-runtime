/**
 * \file linked_list.h
 *
 * \brief linked list
 *
 * The linked list provides forward traversal of a linked list.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_LINKED_LIST_HEADER_GUARD
#define VPR_LINKED_LIST_HEADER_GUARD

#include <vpr/allocator.h>
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
* \param destination   The destination element to which this value will be
*                      copied.
* \param source        The source element used for the copy.
* \param size          The size of the element being copied.
*/
typedef void (*linked_list_element_copy_t)(
    void* destination, const void* source, size_t size);

/**
 * \brief The dispose method to use when disposing an element in this linked
 * list.
 *
 * \param alloc_opts    The allocator options to use.
 * \param elem          The element to be disposed.
 */
typedef void (*linked_list_element_dispose_t)(
    allocator_options_t* alloc_opts, void* elem);

/**
 * \brief This structure contains the options used by a linked list instance.
 *
 * User code will create an options structure using the
 * linked_list_options_init() method declared below.  When this set of
 * options is no longer required, it should be disposed using dispose().
 */
typedef struct linked_list_options
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The allocator options to use when creating linked lists.
     */
    allocator_options_t* alloc_opts;

    /**
     * \brief The element size of lists produced with this set of options.
     */
    size_t element_size;

    /**
     * \brief The copy method to use when copying elements in this linked list.
     *
     * \param destination   The destination element to which this value will be
     *                      copied.
     * \param source        The source element used for the copy.
     * \param size          The size of the element being copied.
     */
    linked_list_element_copy_t linked_list_element_copy;

    /**
     * \brief The dispose method to use when disposing an element in this
     * linked list.
     *
     * \param alloc_opts    The allocator options to use.
     * \param elem          The element to be disposed.
     */
    linked_list_element_dispose_t linked_list_element_dispose;

} linked_list_options_t;

/**
 * \brief This macro defines the model check property for a valid
 * linked_list_options_t structure.
 */
#define MODEL_PROP_VALID_LL_OPTIONS(options, sz) \
    (NULL != options && NULL != (options)->hdr.dispose && NULL != (options)->alloc_opts && (options)->element_size == sz && (NULL == (options)->linked_list_element_copy || sz > 0))

/**
 * \brief An element of the linked list.
 */
typedef struct linked_list_element
{

    /**
     * \brief Pointer to the next element in the list, or NULL
     * if this is the last element.
     */
    struct linked_list_element* next;

    /**
     * \brief Opaque pointer to the data stored in this element.
     */
    void* data;

} linked_list_element_t;


/**
 * \brief This macro defines the model check property for a valid
 * linked_list_element_t structure.
 */
#define MODEL_PROP_VALID_LL_ELEMENT(elem) \
    (NULL != elem && NULL != (elem)->data)


/**
 * \brief The linked list structure.
 *
 */
typedef struct linked_list
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The options used to create this list.
     */
    linked_list_options_t* options;

    /**
     * \brief The number of elements currently instantiated.
     */
    size_t elements;

    /**
     * \brief Pointer to the first element in the list.
     */
    linked_list_element_t* first;

    /**
     * \brief Pointer to the last element in the list.
     */
    linked_list_element_t* last;

} linked_list_t;

/**
 * \brief This macro defines the model check property for a valid
 * linked_list_t structure.
 */
#define MODEL_PROP_VALID_LL(ll) \
    (NULL != ll && NULL != (ll)->hdr.dispose && NULL != (ll)->options && (ll)->elements >= 0)

/**
 * \brief Initialize linked list options for a POD data type.
 *
 * When the function completes successfully, the caller owns this
 * ::linked_list_options_t instance and must dispose of it by calling
 * dispose() when it is no longer needed.
 *
 * \param options           The linked list options to initialize.
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
int linked_list_options_init(
    linked_list_options_t* options, allocator_options_t* alloc_opts,
    bool copy_on_insert, size_t element_size, bool release_on_dispose);

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
    linked_list_element_dispose_t dispose_method);

/**
 * \brief Initialize a linked list.
 *
 * This method allows for the creation of a linked list.  Once initialized, the
 * list will have zero elements, and the first and last pointers will be set to
 * null.
 *
 * When the function completes successfully, the caller owns this
 * ::linked_list_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The linked list options to use for this instance.
 * \param ll                The linked list to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int linked_list_init(linked_list_options_t* options, linked_list_t* ll);

/**
 * \brief Insert a new element at the end of a linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * at the end of the linked list.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted at the
 *                          end of the linked list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero status code on failure.
 */
int linked_list_insert_end(linked_list_t* ll, void* data);

/**
 * \brief Insert a new element after a specified element in a linked list.
 *
 * If successful, then this data will be encapsulated in an element and placed
 * after the specified element in the linked list.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list
 * \param element           The existing element, which will precede the
 *                          new element.
 * \param data              An opaque pointer to some data that should be
 *                          encapsulated in a new element and inserted in the
 *                          list.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - a non-zero status code on failure.
 */
int linked_list_insert_after(
    linked_list_t* ll, linked_list_element_t* element, void* data);

/**
 * \brief Remove an element from a linked list.
 *
 * If successful, the specified element is removed from the linked list.  The
 * caller assumes responsibility for the element and the data referenced by the
 * element.
 *
 * WARNING: this function is not thread safe!
 *
 * \param ll                The linked list.
 * \param element           The element to remove
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 */
int linked_list_remove(linked_list_t* ll, linked_list_element_t* element);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_LINKED_LIST_HEADER_GUARD
