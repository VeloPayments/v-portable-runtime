/**
 * \file compare_bool.c
 *
 * Simple model check of compare_bool.
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

    if (x == y)
    {
        MODEL_ASSERT(compare_bool(&x, &y, sizeof(bool)) == 0);
    }
    else if (x)
    {
        MODEL_ASSERT(x && !y);
        MODEL_ASSERT(compare_bool(&x, &y, sizeof(bool)) > 0);
    }
    else
    {
        MODEL_ASSERT(!x && y);
        MODEL_ASSERT(compare_bool(&x, &y, sizeof(bool)) < 0);
    }

    return 0;
}
