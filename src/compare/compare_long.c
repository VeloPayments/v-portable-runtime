/**
 * \file compare_long.c
 *
 * Implementation of compare_long.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * Compare two signed longs.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_long(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(long));

    long xv = *((long*)x);
    long yv = *((long*)y);

    //we can't use subtraction because of the narrowing conversion from long
    //to int.  Therefore, we need to perform at most two comparisons.
    if (xv > yv)
        return VPR_COMPARE_GREATER;
    if (xv < yv)
        return VPR_COMPARE_LESS;
    else
        return VPR_COMPARE_EQUAL;
}
