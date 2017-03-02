/**
 * \file compare_double.c
 *
 * Simple model check of compare_double.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <math.h>
#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

double nondet_arg1();
double nondet_arg2();

int main(int argc, char* argv[])
{
    double x = nondet_arg1();
    double y = nondet_arg2();

    //Comparisons against NaN are nonsensical.  Exclude these.
    MODEL_ASSUME(!isnan(x) && !isnan(y));

    if (x == y)
    {
        MODEL_ASSERT(compare_double(&x, &y, sizeof(double)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_double(&x, &y, sizeof(double)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_double(&x, &y, sizeof(double)) < 0);
    }

    return 0;
}
