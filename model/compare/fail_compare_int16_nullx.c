/**
 * \file fail_compare_int16_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int16_t nondet_arg2();

int main(int argc, char* argv[])
{
    int16_t y = nondet_arg2();

    compare_int16(NULL, &y, sizeof(int16_t));

    return 0;
}
