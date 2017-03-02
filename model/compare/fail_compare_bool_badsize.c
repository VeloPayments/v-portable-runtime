/**
 * \file fail_compare_bool_badsize.c
 *
 * Passing the wrong size for comparison causes a model exception.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

bool nondet_arg1();
bool nondet_arg2();

int main(int argc, char* argv[])
{
    bool x = nondet_arg1();
    bool y = nondet_arg2();

    compare_bool(&x, &y, sizeof(bool) + 1);

    return 0;
}
