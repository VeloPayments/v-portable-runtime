/**
 * \file fail_compare_unsigned_short_nullx.c
 *
 * A null left hand side argument causes a model assertion.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/compare.h>

unsigned short nondet_arg2();

int main(int argc, char* argv[])
{
    unsigned short y = nondet_arg2();

    compare_unsigned_short(NULL, &y, sizeof(unsigned short));

    return 0;
}
