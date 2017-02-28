/**
 * \file fail_nullcheck.c
 *
 * Verify that releasing a null pointer results in a failure.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
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

    release(&options, foo);  //XXX - this will fail because foo can be NULL

    dispose((disposable_t*)&options);
}
