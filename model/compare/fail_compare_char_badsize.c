/**
 * \file fail_compare_char_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

char nondet_arg1();
char nondet_arg2();

int main(int argc, char* argv[])
{
    char x = nondet_arg1();
    char y = nondet_arg2();

    compare_char(&x, &y, sizeof(char) + 1);

    return 0;
}
