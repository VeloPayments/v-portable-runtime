/**
 * \file fail_compare_uint64_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

uint64_t nondet_arg2();

int main(int argc, char* argv[])
{
    uint64_t y = nondet_arg2();

    compare_uint64(NULL, &y, sizeof(uint64_t));

    return 0;
}
