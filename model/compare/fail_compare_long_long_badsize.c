/**
 * \file fail_compare_long_long_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

long long nondet_arg1();
long long nondet_arg2();

int main(int argc, char* argv[])
{
    long long x = nondet_arg1();
    long long y = nondet_arg2();

    compare_long_long(&x, &y, sizeof(long long) + 1);

    return 0;
}
