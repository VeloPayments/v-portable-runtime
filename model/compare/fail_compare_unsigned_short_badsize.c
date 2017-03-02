/**
 * \file fail_compare_unsigned_short_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned short nondet_arg1();
unsigned short nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned short x = nondet_arg1();
    unsigned short y = nondet_arg2();

    compare_unsigned_short(&x, &y, sizeof(unsigned short) + 1);

    return 0;
}
