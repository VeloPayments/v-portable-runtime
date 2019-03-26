/**
 * \file bloom_filter_options_init_ex.c
 *
 * Implementation of bloom_filter_options_init_ex.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static void bloom_filter_simple_dispose(void*);


/**
 * \brief Initialize a bloom filter.
 *
 * Initialize the bloom filter options using user supplied hash functions.
 * The num_expected_entries parameter should be a reasonable estimate of the
 * upper bound of entries that will be inserted into the filter.  While the
 * filter will continue to operate if more entries are inserted, the error
 * rate will rise above the target error rate.
 *
 * The target_error_rate is the desired rate of false positives, expressed
 * as a percentage in the range (0,1).  The actual error rate is a function
 * of the number of expected entries and the size of the filter.  If the
 * space required fo meet the target error rate is below the specified
 * max_size_in_bytes, then the expected error rate will match the
 * target_error_rate.  If not, the expected error rate will be higher.
 *
 * The supplied hash functions hash_function_1 and hash_function_2 should be
 * capable of hashing a null terminated input value of arbitrary size and
 * producing a 64 bit hashed value.  Hashing is an essential component of the
 * filter; it is imperative that the supplied hash functions be high quality
 * and independent of each other.
 *
 * \param options                  The bloom filter options to initialize.
 * \param alloc_opts               The allocator options to use.
 * \param num_expected_entries     The number of items that are expected to be
 *                                 added to the filter.
 * \param target_error_rate        The desired error rate for false positives.
 * \param max_size_in_bytes        The maximum size the filter is allowed to
 *                                 be.
 * \param hash_function_1          A hash function
 * \param hash_function_2          A hash function
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int bloom_filter_options_init_ex(
    bloom_filter_options_t* options, allocator_options_t* alloc_opts,
    unsigned int num_expected_entries, float target_error_rate,
    size_t max_size_in_bytes, hash_func_t hash_function_1,
    hash_func_t hash_function_2)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(num_expected_entries > 0);
    MODEL_ASSERT(target_error_rate > 0 && target_error_rate < 1.0);
    MODEL_ASSERT(max_size_in_bytes > 0);
    MODEL_ASSERT(NULL != hash_function_1);
    MODEL_ASSERT(NULL != hash_function_2);

    options->hdr.dispose = &bloom_filter_simple_dispose;
    options->alloc_opts = alloc_opts;
    options->num_expected_entries = num_expected_entries;
    options->hash_function_1 = hash_function_1;
    options->hash_function_2 = hash_function_2;

    // given the number of expected entries and the target error rate,
    // calculate the required size of the filter in bits.
    unsigned int m =
        ceil((num_expected_entries * log(target_error_rate)) /
            log(1 / pow(2, log(2))));
    size_t m_bytes;
    if (m % 8)
    {
        m_bytes = m / 8 + 1;
    }
    else
    {
        m_bytes = m / 8;
    }
    options->size_in_bytes = m_bytes < max_size_in_bytes
        ? m_bytes
        : max_size_in_bytes;


    // calculate the number of hash functions
    options->num_hash_functions =
        ceil(((double)m / num_expected_entries) * log(2));

    // calculate the expected error rate, which may not be the same as the
    // target if we couldn't allocate enough space.
    options->expected_error_rate = pow(1 -
            exp(-(float)options->num_hash_functions /
                ((float)options->size_in_bytes * 8 / num_expected_entries)),
        options->num_hash_functions);

    return VPR_STATUS_SUCCESS;
}


/**
 * Dispose of the options structure.  Nothing special needs to be done.
 *
 * \param poptions          Opaque pointer to the options structure.
 */
static void bloom_filter_simple_dispose(void* UNUSED(poptions))
{
    MODEL_ASSERT(poptions != NULL);
}
