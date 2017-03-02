/**
 * \file compare_long_long.c
 *
 * Simple model check of compare_long_long.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

long long nondet_arg1();
long long nondet_arg2();

int main(int argc, char* argv[])
{
    long long x = nondet_arg1();
    long long y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_long_long(&x, &y, sizeof(long long)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_long_long(&x, &y, sizeof(long long)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_long_long(&x, &y, sizeof(long long)) < 0);
    }

    return 0;
}
