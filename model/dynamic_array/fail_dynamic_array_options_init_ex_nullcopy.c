/**
 * \file fail_dynamic_array_options_init_ex_nullcopy.c
 *
 * A null copy method trips the model checker.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/dynamic_array.h>

size_t nondet_element_size();
void* nondet_context();

static void my_dispose(void* context, void* elem)
{
}

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
    size_t sz = nondet_element_size();
    void* ctx = nondet_context();

    //element size must be greater than zero
    MODEL_ASSUME(sz > 0);

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize dynamic array options
    dynamic_array_options_init_ex(
        &options, &alloc_opts, sz, ctx, NULL, &my_dispose, &memcmp);

    //dispose of dynamic array options
    dispose(dynamic_array_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
