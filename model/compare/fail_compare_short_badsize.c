/**
 * \file fail_compare_short_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

short nondet_arg1();
short nondet_arg2();

int main(int argc, char* argv[])
{
    short x = nondet_arg1();
    short y = nondet_arg2();

    compare_short(&x, &y, sizeof(short) + 1);

    return 0;
}
