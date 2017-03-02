/**
 * \file compare_float.c
 *
 * Implementation of compare_float.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * Compare two floats.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_float(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(float));

    float xv = *((float*)x);
    float yv = *((float*)y);

    //we can't use subtraction because of the narrowing conversion from float
    //to int.  Therefore, we need to perform at most two comparisons.
    if (xv > yv)
        return 1;
    if (xv < yv)
        return -1;
    else
        return 0;
}
