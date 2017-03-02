/**
 * \file compare_unsigned_long_long.c
 *
 * Implementation of compare_unsigned_long_long.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * Compare two unsigned long longs.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_long_long(
    const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(unsigned long long));

    unsigned long long xv = *((unsigned long long*)x);
    unsigned long long yv = *((unsigned long long*)y);

    //we can't use subtraction because of the narrowing conversion from long
    //long to int.  Therefore, we need to perform at most two comparisons.
    if (xv > yv)
        return 1;
    if (xv < yv)
        return -1;
    else
        return 0;
}
