/**
 * \file dynamic_array_init.c
 *
 * Implementation of dynamic_array_init.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

//forward decls
static void darr_dispose(void*);

/**
 * Initialize a dynamic array.  This method allows for the creation of a dynamic
 * array that has a reserved size and an optional initial instantiated size.  If
 * the copy element is specified and the instantiated size is greater than zero,
 * then the first N elements will be available in the dynamic array and will be
 * initialized using the copy element.
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
 * \returns zero if successful, non-zero on failure.  On success, the caller
 * owns this dynamic array and must dispose of it by calling dispose() when it
 * is no longer needed.
 */
int dynamic_array_init(
    dynamic_array_options_t* options, dynamic_array_t* array,
    size_t reserve, size_t instance, void* copy)
{
    //the array structure must be non-null
    MODEL_ASSERT(array != NULL);
    //the array must reserve more than 0 arguments.
    MODEL_ASSERT(reserve > 0);
    //either we instantiate zero elements, or the copy element must be non-null
    MODEL_ASSERT(instance == 0 ||
        (copy != NULL && instance > 0 && instance <= reserve));

    //sanity checks on options
    MODEL_ASSERT(options != NULL);
    MODEL_ASSERT(options->alloc_opts != NULL);
    MODEL_ASSERT(options->dynamic_array_element_copy != NULL);

    array->hdr.dispose = &darr_dispose;
    array->options = options;
    array->reserved_elements = reserve;
    array->elements = instance;
    array->array = allocate(options->alloc_opts, reserve * options->element_size);

    //if memory allocation failed, return an error.
    if (array->array == NULL)
    {
        return VPR_ERROR_DYNAMIC_ARRAY_INIT_ALLOCATION_FAILED;
    }

    //instantiate each value in the array with our copy data
    uint8_t* bytearr = (uint8_t*)array->array;
    for (size_t i = 0; i < instance; ++i)
    {
        options->dynamic_array_element_copy(
            options->context, bytearr + i * options->element_size, copy,
            options->element_size);
    }

    //success
    return VPR_STATUS_SUCCESS;
}

/**
 * Dispose of a dynamic array.
 *
 * \param parray        An opaque pointer to the dynamic array.
 */
void darr_dispose(void* parray)
{
    MODEL_ASSERT(parray != NULL);
    dynamic_array_t* array = (dynamic_array_t*)parray;
    MODEL_ASSERT(array->options != NULL);
    MODEL_ASSERT(array->options->alloc_opts != NULL);
    MODEL_ASSERT(array->options->dynamic_array_element_dispose != NULL);
    MODEL_ASSERT(array->array != NULL);
    MODEL_ASSERT(array->elements <= array->reserved_elements);

    //dispose of each element in the array
    uint8_t* barr = (uint8_t*)array->array;
    for (size_t i = 0; i < array->elements; ++i)
    {
        array->options->dynamic_array_element_dispose(
            array->options->context, barr + i * array->options->element_size);
    }

    //release the memory
    release(array->options->alloc_opts, array->array);
}
