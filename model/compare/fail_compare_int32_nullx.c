/**
 * \file fail_compare_int32_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int32_t nondet_arg2();

int main(int argc, char* argv[])
{
    int32_t y = nondet_arg2();

    compare_int32(NULL, &y, sizeof(int32_t));

    return 0;
}
