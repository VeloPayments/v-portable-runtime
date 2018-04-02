/**
 * \file compare_uint32.c
 *
 * Implementation of compare_uint32.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * \brief Compare two uint32_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_uint32(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(uint32_t));

    uint32_t xv = *((uint32_t*)x);
    uint32_t yv = *((uint32_t*)y);

    //per the C specification, there is no guarantee that we can convert
    //uint32_t to int without a narrowing conversion.  Therefore, we must
    //compare these values the long way.
    if (xv > yv)
        return VPR_COMPARE_GREATER;
    else if (xv < yv)
        return VPR_COMPARE_LESS;
    else
        return VPR_COMPARE_EQUAL;
}
