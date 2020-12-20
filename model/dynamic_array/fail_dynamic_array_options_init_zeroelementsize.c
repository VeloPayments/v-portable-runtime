/**
 * \file fail_dynamic_array_options_init_zeroelementsize.c
 *
 * An element size of zero trips the model checker.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/dynamic_array.h>

bool nondet_use_memcmp();

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;
    compare_method_t compare_method = NULL;
    dynamic_array_options_t options;

    //optionally set the comparison argument
    if (nondet_use_memcmp())
        compare_method = &memcmp;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize dynamic array options
    dynamic_array_options_init(&options, &alloc_opts, 0, compare_method);

    //dispose of dynamic array options
    dispose(dynamic_array_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
