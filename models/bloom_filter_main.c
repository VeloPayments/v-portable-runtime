/**
 * \file bloom_filter_main.c
 *
 * Simple model check of bloom filter
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/bloom_filter.h>

static void verify_init();

/**
 * Non-deterministic boolean value, provided by the model checker.
 *
 * \returns true or false in a nondeterministic manner.
 */
_Bool nondet_bool();


int main(int argc, char* argv[])
{
    verify_init();

    return 0;
}

static void verify_init()
{
    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
    bloom_filter_t bloom;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize bloom filter options
    bloom_filter_options_init(&options, &alloc_opts, 64);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_BLOOM_FILTER_OPTIONS(&options));

    //initialize the bloom filter
    if (0 != bloom_filter_init(&options, &bloom))
    {
        goto cleanup_options;
    }

    // verify the bloom filter is valid
    MODEL_ASSERT(MODEL_PROP_VALID_BLOOM_FILTER(&bloom));


    //dispose of the linked list
    dispose((disposable_t*)&bloom);

cleanup_options:
    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}
