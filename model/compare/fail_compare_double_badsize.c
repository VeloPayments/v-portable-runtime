/**
 * \file fail_compare_double_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

double nondet_arg1();
double nondet_arg2();

int main(int argc, char* argv[])
{
    double x = nondet_arg1();
    double y = nondet_arg2();

    compare_double(&x, &y, sizeof(double) + 1);

    return 0;
}
