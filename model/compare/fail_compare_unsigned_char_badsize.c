/**
 * \file fail_compare_unsigned_char_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned char nondet_arg1();
unsigned char nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned char x = nondet_arg1();
    unsigned char y = nondet_arg2();

    compare_unsigned_char(&x, &y, sizeof(unsigned char) + 1);

    return 0;
}
