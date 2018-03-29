/**
 * \file compare_uint64.c
 *
 * Implementation of compare_uint64.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * Compare two uint64_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_uint64(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(uint64_t));

    uint64_t xv = *((uint64_t*)x);
    uint64_t yv = *((uint64_t*)y);

    //per the C specification, there is no guarantee that we can convert
    //uint64_t to int without a narrowing conversion.  Therefore, we must
    //compare these values the long way.
    if (xv > yv)
        return VPR_COMPARE_GREATER;
    else if (xv < yv)
        return VPR_COMPARE_LESS;
    else
        return VPR_COMPARE_EQUAL;
}
