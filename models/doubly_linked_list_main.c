/**
 * \file custom_doubly_linked_list_init.c
 *
 * Simple model check of doubly_linked_list_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize doubly linked list options
    doubly_linked_list_options_init(
        &options, &alloc_opts, sizeof(int));

    //initialize the array
    if (0 != doubly_linked_list_init(&options, &dll))
    {
        return 1;
    }

    //dispose of the linked list
    dispose((disposable_t*)&dll);

    //dispose of dynamic array options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);

    return 0;
}
