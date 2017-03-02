/**
 * \file fail_compare_long_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

long nondet_arg1();
long nondet_arg2();

int main(int argc, char* argv[])
{
    long x = nondet_arg1();
    long y = nondet_arg2();

    compare_long(&x, &y, sizeof(long) + 1);

    return 0;
}
