/**
 * \file fail_dynamic_array_options_init_ex_nullallocator.c
 *
 * Passing in a null allocator will trip the model checker.
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
    dynamic_array_options_t options;
    size_t sz = nondet_element_size();
    void* ctx = nondet_context();

    //element size must be greater than zero
    MODEL_ASSUME(sz > 0);

    //initialize dynamic array options
    dynamic_array_options_init_ex(
        &options, NULL, sz, ctx, &my_copy, &my_dispose, &memcmp);

    //dispose of dynamic array options
    dispose((disposable_t*)&options);

    return 0;
}
