/**
 * \file custom_dynamic_array_grow.c
 *
 * Simple model check of dynamic_array_grow.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/dynamic_array.h>

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
    dynamic_array_t array;
    int SEVENTEEN = 17;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize dynamic array options
    dynamic_array_options_init(
        &options, &alloc_opts, sizeof(int), &compare_int);

    //initialize the array
    if (0 != dynamic_array_init(&options, &array, 3, 2, &SEVENTEEN))
    {
        return 1;
    }

    //grow the array to 5 elements
    if (0 != dynamic_array_grow(&array, 5))
    {
        //dispose of the array
        dispose(dynamic_array_disposable_handle(&array));

        return 2;
    }

    //dispose of the array
    dispose(dynamic_array_disposable_handle(&array));

    //dispose of dynamic array options
    dispose(dynamic_array_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
