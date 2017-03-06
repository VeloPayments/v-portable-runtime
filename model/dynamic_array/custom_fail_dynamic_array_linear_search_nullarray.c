/**
 * \file custom_fail_dynamic_array_linear_search_nullarray.c
 *
 * A null array parameter should trip the model checker.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/dynamic_array.h>

int main(int argc, char* argv[])
{
    int SEVENTEEN = 17;

    //this will trip the model checker
    dynamic_array_linear_search(NULL, NULL, &SEVENTEEN);

    return 0;
}
