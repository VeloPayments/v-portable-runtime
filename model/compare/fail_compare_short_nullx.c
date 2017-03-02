/**
 * \file fail_compare_short_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

short nondet_arg2();

int main(int argc, char* argv[])
{
    short y = nondet_arg2();

    compare_short(NULL, &y, sizeof(short));

    return 0;
}
