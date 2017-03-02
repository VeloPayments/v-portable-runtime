/**
 * \file fail_compare_uint16_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

uint16_t nondet_arg1();
uint16_t nondet_arg2();

int main(int argc, char* argv[])
{
    uint16_t x = nondet_arg1();
    uint16_t y = nondet_arg2();

    compare_uint16(&x, &y, sizeof(uint16_t) + 1);

    return 0;
}
