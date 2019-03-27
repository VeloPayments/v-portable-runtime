/**
 * \file bloom_filter_calculate_size_shadow.c
 *
 * Shadow library implementation of bloom_filter_calculate_size.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <math.h>
#include <vpr/bloom_filter.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_bf_calculate_size_shadowed

unsigned int nondet_uint();

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
    size_t sz = nondet_uint();

    MODEL_ASSUME(sz > 0);

    return sz;
}

#endif /*defined(MODEL_CHECK_vpr_bf_calculate_size_shadowed)*/
