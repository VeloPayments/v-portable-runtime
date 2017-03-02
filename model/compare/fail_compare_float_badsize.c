/**
 * \file fail_compare_float_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

float nondet_arg1();
float nondet_arg2();

int main(int argc, char* argv[])
{
    float x = nondet_arg1();
    float y = nondet_arg2();

    compare_float(&x, &y, sizeof(float) + 1);

    return 0;
}
