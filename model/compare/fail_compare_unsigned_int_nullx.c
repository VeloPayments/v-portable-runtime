/**
 * \file fail_compare_unsigned_int_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned int nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned int y = nondet_arg2();

    compare_unsigned_int(NULL, &y, sizeof(unsigned int));

    return 0;
}
