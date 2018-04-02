/**
 * \file compare_bool.c
 *
 * Implementation of compare_bool.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * \brief Compare two bool values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_bool(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(bool));

    bool xv = *((bool*)x);
    bool yv = *((bool*)y);

    //boolean values are a special case.
    if (xv == yv)
        return VPR_COMPARE_EQUAL;
    else if (xv)
        return VPR_COMPARE_GREATER;
    else
        return VPR_COMPARE_LESS;
}
