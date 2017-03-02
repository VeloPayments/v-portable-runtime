/**
 * \file compare_uint64.c
 *
 * Simple model check of compare_uint64.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

uint64_t nondet_arg1();
uint64_t nondet_arg2();

int main(int argc, char* argv[])
{
    uint64_t x = nondet_arg1();
    uint64_t y = nondet_arg2();

    if (x == y)
    {
        MODEL_ASSERT(compare_uint64(&x, &y, sizeof(uint64_t)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_uint64(&x, &y, sizeof(uint64_t)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_uint64(&x, &y, sizeof(uint64_t)) < 0);
    }

    return 0;
}
