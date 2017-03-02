/**
 * \file compare_int16.c
 *
 * Simple model check of compare_int16.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int16_t nondet_arg1();
int16_t nondet_arg2();

int main(int argc, char* argv[])
{
    int16_t x = nondet_arg1();
    int16_t y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_int16(&x, &y, sizeof(int16_t)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_int16(&x, &y, sizeof(int16_t)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_int16(&x, &y, sizeof(int16_t)) < 0);
    }

    return 0;
}
