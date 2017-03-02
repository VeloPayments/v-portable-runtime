/**
 * \file fail_compare_int16_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int16_t nondet_arg1();

int main(int argc, char* argv[])
{
    int16_t x = nondet_arg1();

    compare_int16(&x, NULL, sizeof(int16_t));

    return 0;
}
