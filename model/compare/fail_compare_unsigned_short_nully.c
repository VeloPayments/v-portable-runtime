/**
 * \file fail_compare_unsigned_short_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned short nondet_arg1();

int main(int argc, char* argv[])
{
    unsigned short x = nondet_arg1();

    compare_unsigned_short(&x, NULL, sizeof(unsigned short));

    return 0;
}
