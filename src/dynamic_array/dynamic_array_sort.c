/**
 * \file dynamic_array_sort.c
 *
 * Implementation of dynamic_array_sort.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/* forward declarations of internal methods */
static int merge_sort(
    dynamic_array_options_t* options, void* input, void* output, size_t size);

/**
 * Sort the given dynamic array.  This method performs a simple merge sort using
 * the comparison method defined in the options structure.
 *
 * If successful, then the array will be sorted.  Note that this merge sort
 * implementation should not be considered a stable sort, so elements that
 * compare as equal may change relative positions as a result of this sort.
 *
 * \param array             The array to be sorted.
 *
 * \returns zero if successful, non-zero on failure.
 */
int dynamic_array_sort(dynamic_array_t* array)
{
    int retval = 0;
    MODEL_ASSERT(array != NULL);
    MODEL_ASSERT(array->array != NULL);
    MODEL_ASSERT(array->elements > 0);
    MODEL_ASSERT(array->reserved_elements >= array->elements);
    MODEL_ASSERT(array->options != NULL);
    MODEL_ASSERT(array->options->alloc_opts != NULL);
    MODEL_ASSERT(array->options->dynamic_array_element_copy != NULL);
    MODEL_ASSERT(array->options->dynamic_array_element_dispose != NULL);
    MODEL_ASSERT(array->options->element_size > 0);

    /* allocate an output array for holding the merged and sorted array. */
    dynamic_array_t output;
    retval = dynamic_array_init(
        array->options, &output, array->reserved_elements, 0, NULL);
    if (VPR_STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* perform the merge sort */
    retval = merge_sort(
        array->options, array->array, output.array, array->elements);
    if (VPR_STATUS_SUCCESS != retval)
    {
        goto dispose_output;
    }

    /* swap the output array and input array buffers */
    void* tmp = array->array;
    array->array = output.array;
    output.array = tmp;

    /* update the output array's elements so dispose properly clears these. */
    output.elements = array->elements;

    /* success */
    retval = VPR_STATUS_SUCCESS;

dispose_output:
    /* dispose of the output array to reclaim memory. */
    dispose((disposable_t*)&output);

    return retval;
}

/**
 * Merge sort.  This sort recursively sorts the left-hand and right-hand sides
 * of the given array, and then merges these sorted results back together.  This
 * results in a maximum stack recursion of O(log n) and a maximum bound of
 * O(n log n) operations to sort the array.
 *
 * \param options       The dynamic array options for this array.
 * \param input         The input array to sort.
 * \param output        The output array where sorted elements are stored.
 * \param size          The size of the input / output arrays.
 */
static int merge_sort(
    dynamic_array_options_t* options, void* input, void* output, size_t size)
{
    int retval = VPR_ERROR_DYNAMIC_ARRAY_SORT_GENERAL;
    uint8_t* in = (uint8_t*)input;
    uint8_t* out = (uint8_t*)output;

    /* if this array is trivially sorted, then copy it to the output and
     * return */
    if (size <= 1)
    {
        for (size_t i = 0; i < size; ++i)
        {
            options->dynamic_array_element_copy(
                options->context,
                out + i * options->element_size,
                in + i * options->element_size,
                options->element_size);
        }

        return VPR_STATUS_SUCCESS;
    }

    /* compute the left-hand and right-hand array sizes */
    size_t lhs_size = size / 2;
    size_t rhs_size = size - lhs_size;

    /* allocate memory for the left-hand and right-hand output arrays */
    uint8_t* lhs_out = (uint8_t*)allocate(options->alloc_opts,
        lhs_size * options->element_size);
    if (lhs_out == NULL)
    {
        return VPR_ERROR_DYNAMIC_ARRAY_SORT_ALLOCATION_FAILED;
    }

    uint8_t* rhs_out = (uint8_t*)allocate(options->alloc_opts,
        rhs_size * options->element_size);
    if (rhs_out == NULL)
    {
        retval = VPR_ERROR_DYNAMIC_ARRAY_SORT_ALLOCATION_FAILED;
        goto dispose_lhs;
    }

    /* recursively sort the left-hand side array */
    retval = merge_sort(options, in, lhs_out, lhs_size);
    if (VPR_STATUS_SUCCESS != retval)
    {
        goto dispose_all;
    }

    /* recursively sort the right-hand side array */
    retval = merge_sort(
        options, in + lhs_size * options->element_size, rhs_out, rhs_size);
    if (VPR_STATUS_SUCCESS != retval)
    {
        goto dispose_all;
    }

    /* now merge the two sorted arrays */
    size_t lhs_idx = 0, rhs_idx = 0, out_idx = 0;
    while (lhs_idx < lhs_size && rhs_idx < rhs_size)
    {
        uint8_t* lhs = lhs_out + lhs_idx * options->element_size;
        uint8_t* rhs = rhs_out + rhs_idx * options->element_size;

        int cmp = options->dynamic_array_element_compare(
            lhs, rhs, options->element_size);

        if (cmp <= 0)
        {
            options->dynamic_array_element_copy(
                options->context,
                out + out_idx * options->element_size,
                lhs,
                options->element_size);

            ++lhs_idx;
        }
        else
        {
            options->dynamic_array_element_copy(
                options->context,
                out + out_idx * options->element_size,
                rhs,
                options->element_size);

            ++rhs_idx;
        }

        ++out_idx;
    }

    /* copy lhs slop */
    if (lhs_idx < lhs_size)
    {
        for (size_t i = lhs_idx; i < lhs_size; ++i)
        {
            options->dynamic_array_element_copy(
                options->context,
                out + out_idx * options->element_size,
                lhs_out + i * options->element_size,
                options->element_size);

            ++out_idx;
        }

        lhs_idx = lhs_size;
    }

    /* copy rhs slop */
    if (rhs_idx < rhs_size)
    {
        for (size_t i = rhs_idx; i < rhs_size; ++i)
        {
            options->dynamic_array_element_copy(
                options->context,
                out + out_idx * options->element_size,
                rhs_out + i * options->element_size,
                options->element_size);

            ++out_idx;
        }

        rhs_idx = rhs_size;
    }

    MODEL_ASSERT(out_idx == size);
    MODEL_ASSERT(lhs_idx == lhs_size);
    MODEL_ASSERT(rhs_idx == rhs_size);

    /* success */
    retval = VPR_STATUS_SUCCESS;

    /* clean up output buffers */
dispose_all:
    release(options->alloc_opts, rhs_out);

dispose_lhs:
    release(options->alloc_opts, lhs_out);

    return retval;
}
