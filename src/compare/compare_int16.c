/**
 * \file compare_int16.c
 *
 * Implementation of compare_int16.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/compare.h>
#include <vpr/parameters.h>

/**
 * \brief Compare two int16_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_int16(const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(int16_t));

    int xv = *((int16_t*)x);
    int yv = *((int16_t*)y);

    return xv - yv;
}
