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
static void verify_add_item();


int main(int argc, char* argv[])
{
    verify_init();
    verify_add_item();

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
    bloom_filter_options_init(&options, &alloc_opts, 1000, 0.1, 1024);

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

static void verify_add_item()
{
    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
    bloom_filter_t bloom;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize bloom filter options
    bloom_filter_options_init(&options, &alloc_opts, 1000, 0.1, 1024);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_BLOOM_FILTER_OPTIONS(&options));

    // initialize the bloom filter
    if (0 != bloom_filter_init(&options, &bloom))
    {
        goto cleanup_options;
    }

    // verify the bloom filter is valid
    MODEL_ASSERT(MODEL_PROP_VALID_BLOOM_FILTER(&bloom));

    uint8_t data[] = { 0x00, 0x11, 0x22 };
    size_t sz_data = sizeof(data);

    MODEL_ASSERT(!bloom_filter_contains_item(&bloom, data, sz_data));

    if (0 == bloom_filter_add_item(&bloom, data, sz_data))
    {
        MODEL_ASSERT(bloom_filter_contains_item(&bloom, data, sz_data));
    }

    MODEL_ASSERT(MODEL_PROP_VALID_BLOOM_FILTER(&bloom));

    //dispose of the linked list
    dispose((disposable_t*)&bloom);

cleanup_options:
    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}
