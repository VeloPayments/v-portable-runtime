/**
 * \file bloom_filter_calculate_size.c
 *
 * Implementation of bloom_filter_calculate_size.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <math.h>
#include <vpr/bloom_filter.h>

/**
 * \brief Helper function to calculate the size of a filter.
 *
 * Calculate the size a bloom filter would need to be to achieve a target
 * error rate, given the number of expected entries.
 *
 * \param num_expected_entries     The number of items that are expected to be
 *                                 added to the filter.
 * \param target_error_rate        The desired error rate for false positives.
 *
 * \return size in bytes the filter would need to be to meet the
 *         target error rate.
 */
size_t bloom_filter_calculate_size(unsigned int num_expected_entries,
    float target_error_rate)
{
    size_t m_bytes;

    unsigned int m_bits = ceil(
        (num_expected_entries * log(target_error_rate)) /
        log(1 / pow(2, log(2))));

    if (m_bits % 8)
    {
        m_bytes = m_bits / 8 + 1;
    }
    else
    {
        m_bytes = m_bits / 8;
    }

    return m_bytes;
}
