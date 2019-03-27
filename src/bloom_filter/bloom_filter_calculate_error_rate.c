/**
 * \file bloom_filter_calculate_error_rate.c
 *
 * Implementation of bloom_filter_calculate_error_rate
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <math.h>
#include <vpr/bloom_filter.h>

/**
 * \brief Helper function to calculate the expected error rate of a bloom
 *        filter.
 *
 * \param num_expected_entries     The number of items that are expected to be
 *                                 added to the filter.
 * \param size_in_bytes            The size of the bloom filter in bytes.
 * \param num_hash_functions       The number of hash functions.
 *
 * \return The number of hash functions required.
 */
float bloom_filter_calculate_expected_error_rate(
    unsigned int num_expected_entries, size_t size_in_bytes,
    unsigned int num_hash_functions)
{
    return pow(1 -
            exp(-(float)num_hash_functions /
                ((float)size_in_bytes * 8 / num_expected_entries)),
        num_hash_functions);
}
