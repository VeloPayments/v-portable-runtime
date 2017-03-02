/**
 * \file fail_compare_bool_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

bool nondet_arg2();

int main(int argc, char* argv[])
{
    bool y = nondet_arg2();

    compare_bool(NULL, &y, sizeof(bool));

    return 0;
}
