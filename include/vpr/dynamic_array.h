/**
 * \file dynamic_array.h
 *
 * \brief Array container for C.
 *
 * The dynamic array provides a simple wrapper around a naked C style array that
 * provides model checking for array bounds, controlled initialization,
 * controlled resizing, sorting, and binary searching.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_DYNAMIC_ARRAY_HEADER_GUARD
#define VPR_DYNAMIC_ARRAY_HEADER_GUARD

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
 * \brief The copy method to use when copying elements in this array.
 *
 * \param context       User-defined context for controlling the copy
 *                      method.
 * \param destination   The destination element to which this value will be
 *                      copied.
 * \param source        The source element used for the copy.
 * \param size          The size of the element being copied.
 */
typedef void (*dynamic_array_element_copy_t)(
    void* context, void* destination, const void* source, size_t size);

/**
 * \brief The dispose method to use when disposing an element in this array.
 *
 * \param context       User-defined context to use for the dispose method.
 * \param elem          The element to be disposed.
 */
typedef void (*dynamic_array_element_dispose_t)(
    void* context, void* elem);

/**
 * \brief This structure contains the options used by a dynamic array instance.
 *
 * User code will create an options structure using either the
 * dynamic_array_options_init() the dynamic_array_options_init_ex() method
 * declared below.  When this set of options is no longer required, it should be
 * disposed using dispose().
 */
typedef struct dynamic_array_options
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The allocator options to use when creating dynamic arrays.
     */
    allocator_options_t* alloc_opts;

    /**
     * \brief The element size of arrays produced with this set of options.
     */
    size_t element_size;

    /**
     * \brief The copy method to use when copying elements in this array.
     *
     * \param context       User-defined context for controlling the copy
     *                      method.
     * \param destination   The destination element to which this value will be
     *                      copied.
     * \param source        The source element used for the copy.
     * \param size          The size of the element being copied.
     */
    dynamic_array_element_copy_t dynamic_array_element_copy;

    /**
     * \brief The dispose method to use when disposing an element in this array.
     *
     * \param context       User-defined context to use for the dispose method.
     * \param elem          The element to be disposed.
     */
    dynamic_array_element_dispose_t dynamic_array_element_dispose;

    /**
     * \brief The comparison method to use when comparing two elements in this
     * array.
     *
     * \param x             The left-hand element.
     * \param y             The right-hand element.
     * \param size          The size of this type.
     *
     * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
     */
    compare_method_t dynamic_array_element_compare;

    /**
     * \brief Context passed to user-defined methods.
     */
    void* context;

} dynamic_array_options_t;

/**
 * \brief The dynamic array structure holds elements of a particular type.
 *
 * The array can be resized, and supports appending, sorting, searching, and
 * copying. The typical usage pattern for this array is to reserve an array of a
 * size estimated to be the size needed, append elements to this array, and then
 * sort the array if needed. Once sorted, the array can be searched using an
 * efficient binary search method.
 *
 * The current use case of this array is to manage the abstract factory pattern
 * and to allow for implementations of interfaces to be registered and later
 * instantiated.
 */
typedef struct dynamic_array
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The options used to create this array.
     */
    dynamic_array_options_t* options;

    /**
     * \brief The reserved size, in elements.
     */
    size_t reserved_elements;

    /**
     * \brief The number of elements currently instantiated.
     */
    size_t elements;

    /**
     * \brief The raw array.
     */
    void* array;

} dynamic_array_t;

/**
 * \brief Initialize dynamic array options for a POD data type.
 *
 * This method should not be used to initialize arrays with data types that
 * cannot be copied directly (e.g. with memcpy()).  An optional argument
 * overrides the comparison method.* If this argument is set to NULL, then
 * memcmp() will be used.  Please see vpr/compare.h for a set of pre-defined
 * comparison methods for most builtin C types. 0
 *
 * When the function completes successfully, the caller owns this
 * ::dynamic_array_t instance and must dispose of it by calling dispose() when
 * it is no longer needed.
 *
 * \param options           The dynamic array options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param element_size      The size of an individual element.
 * \param compare_method    The comparison method to use for sorting, or NULL.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int dynamic_array_options_init(
    dynamic_array_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size, compare_method_t compare_method);

/**
 * \brief Initialize dynamic array options for a custom data type.
 *
 * This method allows the user to override the copy and dispose methods to
 * manage non-POD data types and complex data structures.
 *
 * When the function completes succeffully, the caller owns this
 * ::dynamic_array_t instance and must dispose of it by calling dispose() when
 * it is no longer needed.
 *
 * \param options           The dynamic array options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param element_size      The size of an individual element.
 * \param context           The context value to pass to the copy and dispose
 *                          methods.
 * \param copy_method       The method to use to copy elements.
 * \param dispose_method    The method to use to dispose elements.
 * \param compare_method    The comparison method to use for sorting.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - a non-zero status code on failure.
 */
int dynamic_array_options_init_ex(
    dynamic_array_options_t* options, allocator_options_t* alloc_opts,
    size_t element_size, void* context,
    dynamic_array_element_copy_t copy_method,
    dynamic_array_element_dispose_t dispose_method,
    compare_method_t compare_method);

/**
 * \brief Initialize a dynamic array.
 *
 * This method allows for the creation of a dynamic array that has a reserved
 * size and an optional initial instantiated size.  If the copy element is
 * specified and the instantiated size is greater than zero, then the first N
 * elements will be available in the dynamic array and will be initialized using
 * the copy element.
 *
 * When the function completes succeffully, the caller owns this
 * ::dynamic_array_t instance and must dispose of it by calling dispose() when
 * it is no longer needed.
 *
 * \param options           The dynamic array options to use for this instance.
 * \param array             The array to initialize.
 * \param reserve           The reserve size for this array.
 * \param instance          The optional instance size for this array.  The
 *                          number of reserved elements to instantiate with
 *                          the copy element.
 * \param copy              The optional copy element used for filling the
 *                          initial instances.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_DYNAMIC_ARRAY_INIT_ALLOCATION_FAILED if memory
 *             allocation failed when creating this dynamic array.
 */
int dynamic_array_init(
    dynamic_array_options_t* options, dynamic_array_t* array,
    size_t reserve, size_t instance, void* copy);

/**
 * \brief Request that the reserve size of the dynamic array be increased.
 *
 * This method will attempt to grow the size of the array to the requested size.
 * When this function completes successfully, the reserve size is increased to
 * the given value.
 *
 * \param array             The array to grow.
 * \param reserve           The new reserve size for this array.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_DYNAMIC_ARRAY_GROW_INVALID_ARGUMENT if one of the
 *             provided arguments is invalid or if an attempt is made to shrink
 *             the ::dynamic_array_t instance.
 *      - \ref VPR_ERROR_DYNAMIC_ARRAY_GROW_ALLOCATION_FAILED if an attempt to
 *             allocate a larger array for growing the ::dynamic_array_t
 *             instance fails.
 */
int dynamic_array_grow(dynamic_array_t* array, size_t reserve);

/**
 * \brief Append an element to the end of the dynamic array.
 *
 * If the reserved size is not large enough for this operation, then it will
 * fail.  Users should grow the array using dynamic_array_grow() to change the
 * size of this structure prior to calling this method if the reserved space has
 * been exhausted.
 *
 * If successful, then a copy of this element will be placed at the end of this
 * array using the defined copy method.
 *
 * \param array             The array.
 * \param element           The element to append to this array.
 *
 * \returns a status code indicating success or failure.
 *          - \ref VPR_STATUS_SUCCESS if successful.
 *          - \ref VPR_ERROR_DYNAMIC_ARRAY_APPEND_NO_RESERVE if there is no
 *                 reserve room left for appending this element.
 */
int dynamic_array_append(dynamic_array_t* array, void* element);

/**
 * \brief Sort the given dynamic array.
 *
 * This method performs a simple merge sort using the comparison method defined
 * in the options structure.
 *
 * If successful, then the array will be sorted.  Note that this merge sort
 * implementation should not be considered a stable sort, so elements that
 * compare as equal may change relative positions as a result of this sort.
 *
 * \param array             The array to be sorted.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_DYNAMIC_ARRAY_INIT_ALLOCATION_FAILED if memory
 *             allocation failed when creating a dynamic array for sorting.
 *      - \ref VPR_ERROR_DYNAMIC_ARRAY_SORT_ALLOCATION_FAILED if memory
 *             allocation failed during the merge sort.
 */
int dynamic_array_sort(dynamic_array_t* array);

/**
 * \brief Perform a linear search for an element matching the given key in this
 * array.
 *
 * Note that a linear search will work regardless of whether the array is sorted
 * or not, but it will have a worst-case performance of O(n).  The pointer to
 * the first matching item in the array is returned on success, and NULL is
 * returned on failure.
 *
 * The search method uses the comparison method defined in the array options.
 * How the comparison occurs is up to the comparison method, so this search can
 * be used for key-value pairs.
 *
 * \param array             The array to be sorted.
 * \param compare_method    Optional comparison method override.
 * \param elem              The element to use for searching.
 *
 * \returns a pointer value.
 *      - the first element matching the provided element given the compare
 *        method on success.
 *      - NULL if no matching element found.
 */
void* dynamic_array_linear_search(
    dynamic_array_t* array, compare_method_t compare_method, const void* elem);

/**
 * \brief Perform a binary search for an element matching the given key in this
 * array.
 *
 * Note that a binary search will *only* work if the array is currently sorted.
 * Otherwise, the result of calling this method is unpredictible. This search
 * will have a worst-case performance of O(log n), which is considerably better
 * than a linear search, at the cost of sorting the array first.  As such, this
 * is a suitable method to use for arrays that change rarely and are therefore
 * sorted rarely.
 *
 * The search method uses the comparison method defined in the array options.
 * How the comparison occurs is up to the comparison method, so this search can
 * be used for key-value pairs.
 *
 * \param array             The array to be sorted.
 * \param elem              The element to use for searching.
 *
 * \returns a pointer value.
 *      - the first element matching the provided element given the compare
 *        method on success.
 *      - NULL if no matching element found.
 */
void* dynamic_array_binary_search(dynamic_array_t* array, const void* elem);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_DYNAMIC_ARRAY_HEADER_GUARD
