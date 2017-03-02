/**
 * \file compare_long_double.c
 *
 * Simple model check of compare_long_double.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <math.h>
#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

long double nondet_arg1();
long double nondet_arg2();

int main(int argc, char* argv[])
{
    long double x = nondet_arg1();
    long double y = nondet_arg2();

    //Comparisons against NaN are nonsensical.  Exclude these.
    MODEL_ASSUME(!isnanl(x) && !isnanl(y));

    if (x == y)
    {
        MODEL_ASSERT(compare_long_double(&x, &y, sizeof(long double)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_long_double(&x, &y, sizeof(long double)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_long_double(&x, &y, sizeof(long double)) < 0);
    }

    return 0;
}
