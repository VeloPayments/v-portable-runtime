/**
 * \file dynamic_array_binary_search.c
 *
 * Implementation of dynamic_array_binary_search.
 *
 * \copyright 2021 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

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
void* dynamic_array_binary_search(dynamic_array_t* array, const void* elem)
{
    compare_method_t compare_method;
    MODEL_ASSERT(prop_dynamic_array_valid(array));
    MODEL_ASSERT(NULL != elem);

    /* set the comparison method. */
    compare_method = array->options->dynamic_array_element_compare;

    /* if the compare method does not exist, we cannot continue. */
    MODEL_ASSERT(NULL != compare_method);
    if (NULL == compare_method)
    {
        return NULL;
    }

    /* if there are no elements, return no result. */
    size_t element_size = array->options->element_size;
    if (0 == element_size)
    {
        return NULL;
    }

    /* work with a byte array to make offset calculation easier. */
    uint8_t* byte_array = (uint8_t*)array->array;

    /* set the initial bounds. */
    size_t lower_bound = 0;
    size_t upper_bound = array->elements;

    /* loop over the bounds. */
    while (upper_bound >= lower_bound)
    {
        /* compute the midpoint. */
        size_t midpoint = lower_bound + ((upper_bound - lower_bound) / 2);

        /* calculate the offset of the midpoint element. */
        uint8_t* midpoint_elem = byte_array + midpoint * element_size;

        int compare_result =
            compare_method(midpoint_elem, elem, element_size);

        /* result found. */
        if (VPR_COMPARE_EQUAL == compare_result)
        {
            return midpoint_elem;
        }
        /* midpoint_elem > elem. */
        else if (VPR_COMPARE_GREATER == compare_result)
        {
            upper_bound = midpoint - 1;
        }
        /* midpoint_elem < elem. */
        else
        {
            lower_bound = midpoint + 1;
        }
    }

    /* no results were found. */
    return NULL;
}
