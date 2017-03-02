/**
 * \file fail_compare_int_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

int nondet_arg2();

int main(int argc, char* argv[])
{
    int y = nondet_arg2();

    compare_int(NULL, &y, sizeof(int));

    return 0;
}
