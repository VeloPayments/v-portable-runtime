/**
 * \file fail_nullcheck.c
 *
 * Verify that not checking null on allocation results in a model check failure.
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

    foo = (char*)allocate(&options, 10);
    strcpy(foo, "abcdefghi");  //XXX - should fail (write to NULL)

    if (foo != NULL)
    {
        release(&options, foo);
    }

    dispose(allocator_options_disposable_handle(&options));
}
