/**
 * \file malloc_allocator_model.c
 *
 * Simple model check of the malloc allocator.  Happy path.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/allocator/malloc_allocator.h>

int main(int argc, char* argv[])
{
    allocator_options_t options;
    char* foo;

    malloc_allocator_options_init(&options);

    foo = allocate(&options, 10);
    if (foo != NULL)
    {
        strcpy(foo, "abcdefghi");

        foo = reallocate(&options, foo, 10, 20);
        foo = reallocate(&options, foo, 20, 10);

        release(&options, foo);
    }

    dispose(allocator_options_disposable_handle(&options));
}
