/**
 * \file bloom_filter_options_init.c
 *
 * Implementation of doubly_linked_list_options_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

/* forward decls for internal methods */
static uint64_t sdbm(const void*);
static uint64_t jenkins(const void* data);

/**
 * \brief Initialize a bloom filter options using default hash functions.
 *
 * Initialize the bloom filter options using default hash functions.
 * Those hash functions are sdbm and jenkins.  The num_expected_entries
 * parameter should be a reasonable estimate of the upper bound of entries
 * that will be inserted into the filter.  While the filter will continue
 * to operate if more entries are inserted, the error rate will rise above
 * the target error rate.
 *
 * The target_error_rate is the desired rate of false positives, expressed
 * as a percentage in the range (0,1).  The actual error rate is a function
 * of the number of expected entries and the size of the filter.  If the
 * space required fo meet the target error rate is below the specified
 * max_size_in_bytes, then the expected error rate will match the
 * target_error_rate.  If not, the expected error rate will be higher.
 *
 * \param options                  The bloom filter options to initialize.
 * \param alloc_opts               The allocator options to use.
 * \param num_expected_entries     The number of items that are expected to be
 *                                 added to the filter.
 * \param target_error_rate        The desired error rate for false positives.
 * \param max_size_in_bytes        The maximum size the filter is allowed to
 *                                 be.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int bloom_filter_options_init(
    bloom_filter_options_t* options, allocator_options_t* alloc_opts,
    unsigned int num_expected_entries, float target_error_rate,
    size_t max_size_in_bytes)
{
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != alloc_opts);
    MODEL_ASSERT(num_expected_entries > 0);
    MODEL_ASSERT(target_error_rate > 0 && target_error_rate < 1.0);
    MODEL_ASSERT(max_size_in_bytes > 0);

    return bloom_filter_options_init_ex(
        options, alloc_opts, num_expected_entries, target_error_rate,
        max_size_in_bytes, &sdbm, &jenkins);
}


/**
 * An implementation of the sdbm hash algorithm
 *
 * \param                   The null terminated data to hash
 *
 * \returns The 64 bit hash signature of the input data
 */
static uint64_t sdbm(const void* data)
{
    uint8_t* ptr = (uint8_t*)data;

    uint64_t hash = 5381;
    uint8_t c;

    while ((c = *ptr++))
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }


    return hash;
}

/**
 * An implementation of the jenkins hash algorithm
 *
 * \param                   The null terminated data to hash
 *
 * \returns The 64 bit hash signature of the input data
 */
static uint64_t jenkins(const void* data)
{
    uint8_t* ptr = (uint8_t*)data;

    uint64_t hash = 0;
    uint8_t c;

    while ((c = *ptr++))
    {
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
