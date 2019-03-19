/**
 * \file bloom_filter_options_init_ex.c
 *
 * Implementation of bloom_filter_options_init_ex.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void dll_simple_dispose(void*);


int bloom_filter_options_init_ex(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, size_t size,
    hash_func_t hash_function_1, hash_func_t hash_function_2)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(size > 0);
    MODEL_ASSERT(NULL != hash_function_1);
    MODEL_ASSERT(NULL != hash_function_2);

    //use our dispose method to dispose of these options
    options->hdr.dispose = &dll_simple_dispose;
    //use the user-supplied allocator options
    options->alloc_opts = alloc_opts;
    // set the size of the bitfield
    options->size = size;
    //use the supplied hash functions
    options->hash_function_1 = hash_function_1;
    options->hash_function_2 = hash_function_2;

    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of the options structure.  Nothing special needs to be done.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void dll_simple_dispose(void* UNUSED(poptions))
{
    MODEL_ASSERT(poptions != NULL);
}
