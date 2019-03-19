/**
 * \file bloom_filter.h
 *
 * \brief Bloom filter
 *
 * This bloom filter provides an efficient mechanism for testing set
 * membership.  Note, bloom filters are probabilistic.  False positives are
 * possible, but false negatives are not.  Meaning, if the bloom filter
 * says an element is not in the set, it is certain it is not.  If the
 * bloom filter says an element is in the set, it "probably" is.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_BLOOM_FILTER_HEADER_GUARD
#define VPR_BLOOM_FILTER_HEADER_GUARD

#include <vpr/allocator.h>
#include <vpr/disposable.h>
#include <vpr/error_codes.h>
#include <stdbool.h>
#include <stdlib.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus


typedef uint64_t (*hash_func_t)(const void* data);


typedef struct bloom_filter_options
{

    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The allocator options to use when creating doubly linked lists.
     */
    allocator_options_t* alloc_opts;

    /**
     * \brief The size of the bitfield, in bytes
     */
    size_t size;

    hash_func_t hash_function_1;

    hash_func_t hash_function_2;


} bloom_filter_options_t;

/**
 * \brief This macro defines the model check property for a valid
 * doubly_linked_list_options_t structure.
 */
#define MODEL_PROP_VALID_BLOOM_FILTER_OPTIONS(options) \
    (NULL != options && NULL != (options)->hdr.dispose && NULL != (options)->alloc_opts && (options)->size > 0 && NULL != (options)->hash_function_1 && NULL != (options)->hash_function_2)


/**
 * \brief The bloom filter structure.
 *
 */
typedef struct bloom_filter
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The options used to create this bloom filter.
     */
    bloom_filter_options_t* options;


    /**
     * \brief Opaque pointer to the bitfield used to test
     * membership in a set.
     */
    void* bitmap;

} bloom_filter_t;

/**
 * \brief This macro defines the model check property for a valid
 * bloom_filter_t structure.
 */
#define MODEL_PROP_VALID_BLOOM_FILTER(bloom) \
    (NULL != bloom && NULL != (bloom)->hdr.dispose && NULL != (bloom)->options && NULL != (bloom)->bitmap)


int bloom_filter_options_init(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, size_t size);


int bloom_filter_options_init_ex(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, size_t size,
    hash_func_t hash_function_1, hash_func_t hash_function_2);

int bloom_filter_init(bloom_filter_options_t* options, bloom_filter_t* bloom);

uint64_t bloom_filter_hash(bloom_filter_options_t* options, int n, int m);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_BLOOM_FILTER_HEADER_GUARD
