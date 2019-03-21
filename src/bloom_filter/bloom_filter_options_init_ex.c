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
    allocator_options_t* alloc_opts, size_t size_in_bytes,
    int num_hash_functions, hash_func_t hash_function_1,
    hash_func_t hash_function_2)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(size_in_bytes > 0);
    MODEL_ASSERT(num_hash_functions > 0);
    MODEL_ASSERT(NULL != hash_function_1);
    MODEL_ASSERT(NULL != hash_function_2);

    options->hdr.dispose = &dll_simple_dispose;
    options->alloc_opts = alloc_opts;
    options->size_in_bytes = size_in_bytes;
    options->num_hash_functions = num_hash_functions;
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
