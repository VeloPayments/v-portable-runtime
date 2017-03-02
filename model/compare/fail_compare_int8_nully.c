/**
 * \file fail_compare_int8_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int8_t nondet_arg1();

int main(int argc, char* argv[])
{
    int8_t x = nondet_arg1();

    compare_int8(&x, NULL, sizeof(int8_t));

    return 0;
}
