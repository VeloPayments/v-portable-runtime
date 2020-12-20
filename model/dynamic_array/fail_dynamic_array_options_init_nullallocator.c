/**
 * \file fail_dynamic_array_options_init_nullallocator.c
 *
 * Passing in a null allocator trips the model checker.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/dynamic_array.h>

size_t nondet_element_size();
bool nondet_use_memcmp();

int main(int argc, char* argv[])
{
    compare_method_t compare_method = NULL;
    dynamic_array_options_t options;
    size_t sz = nondet_element_size();

    //element size must be greater than zero
    MODEL_ASSUME(sz > 0);

    //optionally set the comparison argument
    if (nondet_use_memcmp())
        compare_method = &memcmp;

    //initialize dynamic array options
    dynamic_array_options_init(&options, NULL, sz, compare_method);

    //dispose of dynamic array options
    dispose(dynamic_array_options_disposable_handle(&options));

    return 0;
}
