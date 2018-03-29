/**
 * \file compare_long_double.c
 *
 * Implementation of compare_long_double.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * Compare two long doubles.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_long_double(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(long double));

    long double xv = *((long double*)x);
    long double yv = *((long double*)y);

    //we can't use subtraction because of the narrowing conversion from float
    //to int.  Therefore, we need to perform at most two comparisons.
    if (xv > yv)
        return VPR_COMPARE_GREATER;
    if (xv < yv)
        return VPR_COMPARE_LESS;
    else
        return VPR_COMPARE_EQUAL;
}
