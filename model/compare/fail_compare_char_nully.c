/**
 * \file fail_compare_char_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

char nondet_arg1();

int main(int argc, char* argv[])
{
    char x = nondet_arg1();

    compare_char(&x, NULL, sizeof(char));

    return 0;
}
