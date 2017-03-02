/**
 * \file compare_int8.c
 *
 * Simple model check of compare_int8.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int8_t nondet_arg1();
int8_t nondet_arg2();

int main(int argc, char* argv[])
{
    int8_t x = nondet_arg1();
    int8_t y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_int8(&x, &y, sizeof(int8_t)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_int8(&x, &y, sizeof(int8_t)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_int8(&x, &y, sizeof(int8_t)) < 0);
    }

    return 0;
}
