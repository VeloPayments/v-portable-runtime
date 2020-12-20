/**
 * \file fail_dynamic_array_options_init_ex_elementsize.c
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

void* nondet_context();

static void my_copy(void* context, void* dst, const void* source, size_t size)
{
}

static void my_dispose(void* context, void* elem)
{
}

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
    void* ctx = nondet_context();

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize dynamic array options
    dynamic_array_options_init_ex(
        &options, &alloc_opts, 0, ctx, &my_copy, &my_dispose, &memcmp);

    //dispose of dynamic array options
    dispose(dynamic_array_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
