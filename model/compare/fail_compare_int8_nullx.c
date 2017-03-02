/**
 * \file fail_compare_int8_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int8_t nondet_arg2();

int main(int argc, char* argv[])
{
    int8_t y = nondet_arg2();

    compare_int8(NULL, &y, sizeof(int8_t));

    return 0;
}
