/**
 * \file compare_short.c
 *
 * Simple model check of compare_short.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

short nondet_arg1();
short nondet_arg2();

int main(int argc, char* argv[])
{
    short x = nondet_arg1();
    short y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_short(&x, &y, sizeof(short)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_short(&x, &y, sizeof(short)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_short(&x, &y, sizeof(short)) < 0);
    }

    return 0;
}
