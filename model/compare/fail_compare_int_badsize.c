/**
 * \file fail_compare_int_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int nondet_arg1();
int nondet_arg2();

int main(int argc, char* argv[])
{
    int x = nondet_arg1();
    int y = nondet_arg2();

    compare_int(&x, &y, sizeof(int) + 1);

    return 0;
}
