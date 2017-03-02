/**
 * \file fail_compare_uint32_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

uint32_t nondet_arg2();

int main(int argc, char* argv[])
{
    uint32_t y = nondet_arg2();

    compare_uint32(NULL, &y, sizeof(uint32_t));

    return 0;
}
