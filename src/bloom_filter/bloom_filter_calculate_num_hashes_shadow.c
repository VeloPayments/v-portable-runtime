/**
 * \file bloom_filter_calculate_num_hashes_shadow.c
 *
 * Shadow library implementation of bloom_filter_calculate_num_hashes.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <math.h>
#include <vpr/bloom_filter.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_bf_calculate_hashes_shadowed

unsigned int nondet_uint();

/**
 * \brief Helper function to calculate the number of hash functions to use.
 *
 * \param num_expected_entries     The number of items that are expected to be
 *                                 added to the filter.
 * \param size_in_bytes            The size of the bloom filter in bytes.
 *
 * \return The number of hash functions required.
 */
unsigned int bloom_filter_calculate_num_hashes(
    size_t num_expected_entries, size_t size_in_bytes)
{
    unsigned int n = nondet_uint();

    MODEL_ASSUME(n > 0);

    return n;
}

#endif /*defined(MODEL_CHECK_vpr_bf_calculate_hashes_shadowed)*/
