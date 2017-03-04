/**
 * \file custom_fail_dynamic_array_init_nulloptions.c
 *
 * A null options parameter trips model check.
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
    dynamic_array_t array;
    int SEVENTEEN = 17;

    //initialize the array
    dynamic_array_init(NULL, &array, 10, 3, &SEVENTEEN);

    return 0;
}
