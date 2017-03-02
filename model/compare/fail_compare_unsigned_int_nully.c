/**
 * \file fail_compare_unsigned_int_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned int nondet_arg1();

int main(int argc, char* argv[])
{
    unsigned int x = nondet_arg1();

    compare_unsigned_int(&x, NULL, sizeof(unsigned int));

    return 0;
}
