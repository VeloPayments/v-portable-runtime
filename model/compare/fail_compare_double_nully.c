/**
 * \file fail_compare_double_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

double nondet_arg1();

int main(int argc, char* argv[])
{
    double x = nondet_arg1();

    compare_double(&x, NULL, sizeof(double));

    return 0;
}
