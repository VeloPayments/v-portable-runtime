/**
 * \file fail_nullcheck.c
 *
 * Verify that allocating a zero byte region fails.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <cbmc/model_assert.h>
#include <vpr/allocator/malloc_allocator.h>

int main(int argc, char* argv[])
{
    allocator_options_t options;
    char* foo = NULL;

    malloc_allocator_options_init(&options);

    foo = (char*)allocate(&options, 0);  //XXX - this will fail because size == 0

    if (foo != NULL)
    {
        release(&options, foo);
    }

    dispose((disposable_t*)&options);
}
