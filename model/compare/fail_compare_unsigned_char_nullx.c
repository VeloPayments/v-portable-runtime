/**
 * \file fail_compare_unsigned_char_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned char nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned char y = nondet_arg2();

    compare_unsigned_char(NULL, &y, sizeof(unsigned char));

    return 0;
}
