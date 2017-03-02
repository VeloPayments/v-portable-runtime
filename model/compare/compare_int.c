/**
 * \file compare_int.c
 *
 * Simple model check of compare_int.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int nondet_arg1();
int nondet_arg2();

int main(int argc, char* argv[])
{
    int x = nondet_arg1();
    int y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_int(&x, &y, sizeof(int)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_int(&x, &y, sizeof(int)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_int(&x, &y, sizeof(int)) < 0);
    }

    return 0;
}
