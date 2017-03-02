/**
 * \file fail_compare_double_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

double nondet_arg2();

int main(int argc, char* argv[])
{
    double y = nondet_arg2();

    compare_double(NULL, &y, sizeof(double));

    return 0;
}
