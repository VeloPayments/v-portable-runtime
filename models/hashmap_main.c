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
static void verify_put();
static void verify_get();

int main(int argc, char* argv[])
{
    verify_empty_hashmap();
    verify_put();
    verify_get();

    return 0;
}

static void verify_empty_hashmap()
{
    allocator_options_t alloc_opts;
    hashmap_options_t options;
    hashmap_t hmap;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize hashmap options
    hashmap_options_init(
        &options, &alloc_opts, 1, false, sizeof(int), false);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_HASHMAP_OPTIONS(&options, sizeof(int)));

    //initialize the hashmap
    if (0 != hashmap_init(&options, &hmap))
    {
        goto cleanup_options;
    }

    // verify the (empty) hashmap is valid
    MODEL_ASSERT(MODEL_PROP_VALID_HASHMAP(&hmap));

    //dispose of the hashmap
    dispose((disposable_t*)&hmap);

cleanup_options:
    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}

static void verify_put()
{
    allocator_options_t alloc_opts;
    hashmap_options_t options;
    hashmap_t hmap;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize hashmap options
    if (0 != hashmap_options_init(&options, &alloc_opts, 1, true, sizeof(int), false))
    {
        goto cleanup_alloc_opts;
    }

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_HASHMAP_OPTIONS(&options, sizeof(int)));

    //initialize the hashmap
    if (0 != hashmap_init(&options, &hmap))
    {
        goto cleanup_options;
    }

    // add an item
    uint64_t key = (uint64_t)45;
    int value = 3;
    hashmap_put(&hmap, key, &value);

    //dispose of the hashmap
    dispose((disposable_t*)&hmap);

cleanup_options:
    //dispose of options
    dispose((disposable_t*)&options);

cleanup_alloc_opts:
    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}

static void verify_get()
{
    allocator_options_t alloc_opts;
    hashmap_options_t options;
    hashmap_t hmap;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize hashmap options
    if (0 != hashmap_options_init(&options, &alloc_opts, 1, false, sizeof(int), false))
    {
        goto cleanup_alloc_opts;
    }

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_HASHMAP_OPTIONS(&options, sizeof(int)));

    //initialize the hashmap
    if (0 != hashmap_init(&options, &hmap))
    {
        goto cleanup_options;
    }

    // GET  an item
    uint64_t key = (uint64_t)45;
    int* val = (int*)hashmap_get(&hmap, key);

    //dispose of the hashmap
    dispose((disposable_t*)&hmap);

cleanup_options:
    //dispose of options
    dispose((disposable_t*)&options);

cleanup_alloc_opts:
    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}
