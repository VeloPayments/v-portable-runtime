/**
 * \file fail_compare_uint32_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

uint32_t nondet_arg1();

int main(int argc, char* argv[])
{
    uint32_t x = nondet_arg1();

    compare_uint32(&x, NULL, sizeof(uint32_t));

    return 0;
}
