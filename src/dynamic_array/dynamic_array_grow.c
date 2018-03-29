/**
 * \file dynamic_array_grow.c
 *
 * Implementation of dynamic_array_grow.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/**
 * Request that the reserve size of the dynamic array be increased.  This method
 * will attempt to grow the size of the array to the requested size.
 *
 * \param array             The array to grow.
 * \param reserve           The new reserve size for this array.
 *
 * \returns zero if successful, non-zero on failure.  On success, the reserve
 * size is increased to the given value.
 */
int dynamic_array_grow(dynamic_array_t* array, size_t reserve)
{
    MODEL_ASSERT(array != NULL);
    MODEL_ASSERT(array->array != NULL);
    MODEL_ASSERT(reserve > array->reserved_elements);
    MODEL_ASSERT(array->options != NULL);
    MODEL_ASSERT(array->options->alloc_opts != NULL);
    MODEL_ASSERT(array->options->dynamic_array_element_copy != NULL);
    MODEL_ASSERT(array->options->dynamic_array_element_dispose != NULL);
    MODEL_ASSERT(array->options->element_size > 0);

    //we can't shrink the array.
    if (reserve <= array->reserved_elements)
    {
        return VPR_ERROR_DYNAMIC_ARRAY_GROW_INVALID_ARGUMENT;
    }

    //the resize strategy is simple: we allocate a buffer large enough for the
    //new reserve size, then copy any instantiated elements to the new buffer
    //using the user-supplied copy methods.  Finally, we dispose the old
    //elements in the old buffer, swap in the new buffer, and free the old
    //buffer.  We don't use the reallocate() method provided in our allocator
    //interface because this assumes that the underlying data can be copied as
    //POD.  This method is safer, as it allows the user-supplied copy and
    //dispose methods to handle internal references that may depend upon the
    //underlying structure of memory.

    uint8_t* old_buffer = (uint8_t*)array->array;
    uint8_t* new_buffer =
        (uint8_t*)allocate(
            array->options->alloc_opts,
            reserve * array->options->element_size);

    //return a failure if memory allocation failed.
    if (new_buffer == NULL)
    {
        return VPR_ERROR_DYNAMIC_ARRAY_GROW_ALLOCATION_FAILED;
    }

    //our pointers are now valid, and by contract, both old_buffer and
    //new_buffer have at least array->reserved_elements capacity, which is less
    //than or equal to array->elements size.
    MODEL_ASSERT(old_buffer != NULL);
    MODEL_ASSERT(new_buffer != NULL);
    MODEL_ASSERT(array->elements <= array->reserved_elements);

    //copy the data from the old buffer to the new buffer
    for (size_t i = 0; i < array->elements; ++i)
    {
        array->options->dynamic_array_element_copy(
            array->options->context,
            new_buffer + i * array->options->element_size,
            old_buffer + i * array->options->element_size,
            array->options->element_size);
    }

    //the new buffer is now our array
    array->reserved_elements = reserve;
    array->array = new_buffer;

    //dispose the elements in the old array
    for (size_t i = 0; i < array->elements; ++i)
    {
        array->options->dynamic_array_element_dispose(
            array->options->context,
            old_buffer + i * array->options->element_size);
    }

    //free the old array
    release(array->options->alloc_opts, old_buffer);

    return VPR_STATUS_SUCCESS;
}
