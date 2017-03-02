/**
 * \file fail_compare_unsigned_int_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned int nondet_arg1();
unsigned int nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned int x = nondet_arg1();
    unsigned int y = nondet_arg2();

    compare_unsigned_int(&x, &y, sizeof(unsigned int) + 1);

    return 0;
}
