/**
 * \file custom_fail_dynamic_array_grow_nullarray.c
 *
 * If the array parameter is null, then the model checker is tripped.
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
    //attempt to grow a null array.  This will trip the model checker.
    dynamic_array_grow(NULL, 2);

    return 0;
}
