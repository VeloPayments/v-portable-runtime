/**
 * \file fail_compare_short_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

short nondet_arg1();

int main(int argc, char* argv[])
{
    short x = nondet_arg1();

    compare_short(&x, NULL, sizeof(short));

    return 0;
}
