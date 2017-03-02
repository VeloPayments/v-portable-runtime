/**
 * \file fail_compare_bool_nully.c
 *
 * A null right hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

bool nondet_arg1();

int main(int argc, char* argv[])
{
    bool x = nondet_arg1();

    compare_bool(&x, NULL, sizeof(bool));

    return 0;
}
