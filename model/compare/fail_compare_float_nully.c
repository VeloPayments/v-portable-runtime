/**
 * \file fail_compare_float_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

float nondet_arg1();

int main(int argc, char* argv[])
{
    float x = nondet_arg1();

    compare_float(&x, NULL, sizeof(float));

    return 0;
}
