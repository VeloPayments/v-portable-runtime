/**
 * \file dynamic_array_linear_search.c
 *
 * Implementation of dynamic_array_linear_search.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/**
 * Perform a linear search for an element matching the given key in this
 * array.  Note that a linear search will work regardless of whether the array
 * is sorted or not, but it will have a worst-case performance of O(n).  The
 * pointer to the first matching item in the array is returned on success, and
 * NULL is returned on failure.
 *
 * The search method uses the comparison method defined in the array options.
 * How the comparison occurs is up to the comparison method, so this search can
 * be used for key-value pairs.
 *
 * \param array             The array to be sorted.
 * \param compare_method    Optional comparison method override.
 * \param elem              The element to use for searching.
 *
 * \returns zero if successful, non-zero on failure.
 */
void* dynamic_array_linear_search(
    dynamic_array_t* array, compare_method_t compare_method, const void* elem)
{
    MODEL_ASSERT(elem != NULL);
    MODEL_ASSERT(array != NULL);
    MODEL_ASSERT(array->array != NULL);
    MODEL_ASSERT(array->options != NULL);

    //if the comparison method is not set, then use the default comparison
    //method.
    if (compare_method == NULL)
    {
        compare_method = array->options->dynamic_array_element_compare;

        //if we have no comparison method, we can't continue.
        MODEL_ASSERT(compare_method != NULL);
        if (compare_method == NULL)
        {
            return NULL;
        }
    }

    //attempt to find a matching element
    uint8_t* byteArray = (uint8_t*)array->array;
    for (size_t i = 0; i < array->elements; ++i)
    {
        uint8_t* lhs = byteArray + i * array->options->element_size;

        if (0 == compare_method(lhs, elem, array->options->element_size))
        {
            //result found
            return lhs;
        }
    }

    //no result found
    return NULL;
}
