/**
 * \file compare_char.c
 *
 * Simple model check of compare_char.
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

    if (x == y)
    {
        MODEL_ASSERT(compare_char(&x, &y, sizeof(char)) == 0);
    }
    else if (x > y)
    {
        MODEL_ASSERT(compare_char(&x, &y, sizeof(char)) > 0);
    }
    else
    {
        MODEL_ASSERT(x < y);
        MODEL_ASSERT(compare_char(&x, &y, sizeof(char)) < 0);
    }

    return 0;
}
