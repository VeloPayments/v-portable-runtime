/**
 * \file fail_compare_long_long_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

long long nondet_arg2();

int main(int argc, char* argv[])
{
    long long y = nondet_arg2();

    compare_long_long(NULL, &y, sizeof(long long));

    return 0;
}
