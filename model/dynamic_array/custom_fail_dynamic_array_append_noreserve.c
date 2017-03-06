/**
 * \file custom_fail_dynamic_array_append_noreserve.c
 *
 * If there isn't enough reserved space for an append, this will trip the model
 * checker.
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
    if (0 != dynamic_array_init(&options, &array, 1, 1, &SEVENTEEN))
    {
        return 1;
    }

    //append an element to the array.  This will trip the model checker because
    //there isn't enough reserved space.
    dynamic_array_append(&array, &SEVENTEEN);

    //dispose of the array
    dispose((disposable_t*)&array);

    //dispose of dynamic array options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);

    return 0;
}
