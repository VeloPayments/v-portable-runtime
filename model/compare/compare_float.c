/**
 * \file compare_float.c
 *
 * Simple model check of compare_float.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <math.h>
#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

float nondet_arg1();
float nondet_arg2();

int main(int argc, char* argv[])
{
    float x = nondet_arg1();
    float y = nondet_arg2();

    //Comparisons against NaN are nonsensical.  Exclude these.
    MODEL_ASSUME(!isnanf(x) && !isnanf(y));

    if (x == y)
    {
        MODEL_ASSERT(compare_float(&x, &y, sizeof(float)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_float(&x, &y, sizeof(float)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_float(&x, &y, sizeof(float)) < 0);
    }

    return 0;
}
