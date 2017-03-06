/**
 * \file dynamic_array_append.c
 *
 * Implementation of dynamic_array_append.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/**
 * Append an element to the end of the dynamic array.  If the reserved size is
 * not large enough for this operation, then it will fail.  Users should grow
 * the array using dynamic_array_grow() to change the size of this structure
 * prior to calling this method if the reserved space has been exhausted.
 *
 * If successful, then a copy of this element will be placed at the end of this
 * array using the defined copy method.
 *
 * \param array             The array.
 * \param element           The element to append to this array.
 *
 * \returns zero if successful, non-zero on failure.
 */
int dynamic_array_append(dynamic_array_t* array, void* element)
{
    MODEL_ASSERT(element != NULL);
    MODEL_ASSERT(array != NULL);
    MODEL_ASSERT(array->array != NULL);
    MODEL_ASSERT(array->options != NULL);
    MODEL_ASSERT(array->options->element_size > 0);
    MODEL_ASSERT(array->options->dynamic_array_element_copy != NULL);
    MODEL_ASSERT(array->elements < array->reserved_elements);

    //we need at least one reserved slot for this append to work
    if (array->elements >= array->reserved_elements)
    {
        return 1;
    }

    //copy the element to the end of the array
    uint8_t* byteArray = (uint8_t*)array->array;
    array->options->dynamic_array_element_copy(
        array->options->context,
        byteArray + array->elements * array->options->element_size,
        element,
        array->options->element_size);
    //increment the number of elements in the array
    ++array->elements;

    //as a post-condition, the element size should not exceed the reserved size
    MODEL_ASSERT(array->elements <= array->reserved_elements);

    return 0;
}
