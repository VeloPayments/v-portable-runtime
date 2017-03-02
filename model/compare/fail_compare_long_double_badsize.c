/**
 * \file fail_compare_long_double_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

long double nondet_arg1();
long double nondet_arg2();

int main(int argc, char* argv[])
{
    long double x = nondet_arg1();
    long double y = nondet_arg2();

    compare_long_double(&x, &y, sizeof(long double) + 1);

    return 0;
}
