/**
 * \file fail_compare_float_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

float nondet_arg2();

int main(int argc, char* argv[])
{
    float y = nondet_arg2();

    compare_float(NULL, &y, sizeof(float));

    return 0;
}
