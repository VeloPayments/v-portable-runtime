/**
 * \file fail_zeroreallocate.c
 *
 * Verify that reallocating to zero bytes fails.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/allocator/malloc_allocator.h>

int main(int argc, char* argv[])
{
    allocator_options_t options;
    char* foo = NULL;

    malloc_allocator_options_init(&options);

    foo = (char*)allocate(&options, 10);

    if (foo != NULL)
    {
        foo = reallocate(&options, foo, 10, 0);  //XXX - newsize 0 == fail

        release(&options, foo);
    }

    dispose(allocator_options_disposable_handle(&options));
}
