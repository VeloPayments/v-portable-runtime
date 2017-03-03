/**
 * \file fail_dynamic_array_options_init_ex_nulloptions.c
 *
 * Verify that passing a null in for options fails the model check.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/dynamic_array.h>

size_t nondet_element_size();
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
    size_t sz = nondet_element_size();
    void* ctx = nondet_context();

    //element size must be greater than zero
    MODEL_ASSUME(sz > 0);

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //This will fail because options is NULL.
    dynamic_array_options_init_ex(
        NULL, &alloc_opts, sz, ctx, &my_copy, &my_dispose, &memcmp);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);

    return 0;
}
