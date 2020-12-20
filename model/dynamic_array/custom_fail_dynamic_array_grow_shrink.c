/**
 * \file custom_fail_dynamic_array_grow_shrink.c
 *
 * If an attempt is made to shrink the dynamic array, this trips the model
 * checker.
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

    //Attempt to shrink the array to 2 elements - this will trip the model
    //checker
    dynamic_array_grow(&array, 2);

    //dispose of the array
    dispose(dynamic_array_disposable_handle(&array));

    //dispose of dynamic array options
    dispose(dynamic_array_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
