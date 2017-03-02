/**
 * \file compare_unsigned_long.c
 *
 * Simple model check of compare_unsigned_long.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned long nondet_arg1();
unsigned long nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned long x = nondet_arg1();
    unsigned long y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_unsigned_long(&x, &y, sizeof(unsigned long)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_unsigned_long(&x, &y, sizeof(unsigned long)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_unsigned_long(&x, &y, sizeof(unsigned long)) < 0);
    }

    return 0;
}
