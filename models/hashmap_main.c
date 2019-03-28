/**
 * \file hashmap_main.c
 *
 * Simple model check of hashmap
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/hashmap.h>

static void verify_empty_hashmap();

/**
 * Non-deterministic boolean value, provided by the model checker.
 *
 * \returns true or false in a nondeterministic manner.
 */
_Bool nondet_bool();


int main(int argc, char* argv[])
{
    verify_empty_hashmap();

    return 0;
}

static void verify_empty_hashmap()
{
    allocator_options_t alloc_opts;
    hashmap_options_t options;
    hashmap_t hmap;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize doubly linked list options
    hashmap_options_init(
        &options, &alloc_opts, 1024u);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_HASHMAP_OPTIONS(&options));

    //initialize the hashmap
    if (0 != hashmap_init(&options, &hmap))
    {
        goto cleanup_options;
    }

    // verify the (empty) hashmap is valid
    MODEL_ASSERT(0 == hmap.elements);
    MODEL_ASSERT(MODEL_PROP_VALID_HASHMAP(&hmap));

    //dispose of the hashmap
    dispose((disposable_t*)&hmap);

cleanup_options:
    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}
