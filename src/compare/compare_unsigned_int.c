/**
 * \file compare_unsigned_int.c
 *
 * Implementation of compare_unsigned_int.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * Compare two unsigned ints.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_int(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(unsigned int));

    unsigned int xv = *((unsigned int*)x);
    unsigned int yv = *((unsigned int*)y);

    //we can't use subtraction because of the narrowing conversion from unsigned
    //to signed.  Therefore, we need to perform at most two comparisons.
    if (xv > yv)
        return 1;
    if (xv < yv)
        return -1;
    else
        return 0;
}
