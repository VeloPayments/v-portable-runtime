/**
 * \file bloom_filter_calculate_num_hashes.c
 *
 * Implementation of bloom_filter_calculate_num_hashes.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <math.h>
#include <vpr/bloom_filter.h>

/* this is the real implementation. */
#ifndef MODEL_CHECK_vpr_bf_calculate_hashes_shadowed

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
    unsigned int num_expected_entries, size_t size_in_bytes)
{

    return round(
        ((double)(size_in_bytes * 8) / num_expected_entries) * log(2));
}

#endif /*!defined(MODEL_CHECK_vpr_bf_calculate_hashes_shadowed)*/
