/**
 * \file fail_compare_int32_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int32_t nondet_arg1();
int32_t nondet_arg2();

int main(int argc, char* argv[])
{
    int32_t x = nondet_arg1();
    int32_t y = nondet_arg2();

    compare_int32(&x, &y, sizeof(int32_t) + 1);

    return 0;
}
