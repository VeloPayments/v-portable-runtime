/**
 * \file bloom_filter.h
 *
 * \brief Bloom filter
 *
 * This bloom filter provides an efficient mechanism for testing set
 * membership.  Note, bloom filters are probabilistic.  False positives are
 * possible, but false negatives are not.  Meaning, if the bloom filter
 * says an element is not in the set, it is certain it is not.  If the
 * bloom filter says an element is in the set, it "probably" is.  The
 * probability of a false positive is a function of the size of the filter
 * and the number of items inserted, and is configurable.
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


/**
 * \brief A hash function that hashes an arbitrary amount of data into a
 * 64 bit value.
 *
 * The bloom filter uses two hash functions to generate an artibrary
 * number of additional hash functions.  Good hash functions are an essential
 * component of the bloom filter.
 *
 * To minimize the error rate of the filter, each hash function should
 * 1) be completely determined by the input value (and therefore repeatable)
 * 2) use the entire input value
 * 3) uniformly distribute the hash values over the set of possible hash values
 *
 * Additionally, each hash function should be independent of the other.  Call
 * these hash functions hf0 and hf1.  For any given input value iv, the value
 * of each bit of hf0(iv) should have a 50% probability of being different
 * than the same bit of hf1(iv).
 *
 * \param data   The null terminated data to be hashed.
 *
 * \returns a 64 bit hash value
 */
typedef uint64_t (*hash_func_t)(const void* data);

/**
 * \brief This structure contains the options used by a bloom filter instance.
 *
 * User code will create an options structure using the
 * bloom_filter_options_init() method declared below.  When this set of options
 * is no longer required, it should be disposed using dispose().
 */
typedef struct bloom_filter_options
{

    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The allocator options to use when creating a bloom filter.
     */
    allocator_options_t* alloc_opts;

    /**
     * \brief The number of expected entries into the filter.
     */
    unsigned int num_expected_entries;

    /**
     * \brief The size in bytes of the filter.
     *
     * This is the "m" value in the literature, though most references
     * refer to m as the size in bits.
     */
    size_t size_in_bytes;

    /**
     * \brief The number of hash functions to use when storing an item in the
     * filter.
     *
     * This is the "k" value in the literature.  The value should be a small
     * positive value.  Each of the K hash functions is derived using two
     * supplied hash functions.
     */
    unsigned int num_hash_functions;

    /**
     * \brief The expeted rate of false positives, expressed as a percentage
     * in the range (0,1).
     *
     * This is the "p" value in the literature.
     */
    float expected_error_rate;

    /**
     * \brief The first of two hash functions, used to derive additional hash
     * functions.
     */
    hash_func_t hash_function_1;

    /**
     * \brief The second of two hash functions, used to derive additional hash
     * functions.
     */
    hash_func_t hash_function_2;


} bloom_filter_options_t;

/**
 * \brief This macro defines the model check property for a valid
 * bloom_filter_options_t structure.
 */
#define MODEL_PROP_VALID_BLOOM_FILTER_OPTIONS(options) \
    (NULL != options && NULL != (options)->hdr.dispose && NULL != (options)->alloc_opts && (options)->size_in_bytes > 0 && (options)->num_hash_functions > 0 && (options)->expected_error_rate >= 0.0 && (options)->expected_error_rate <= 1.0 && NULL != (options)->hash_function_1 && NULL != (options)->hash_function_2)


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
     * \brief Opaque pointer to the bitfield used to test membership in a set.
     */
    void* bitmap;

} bloom_filter_t;

/**
 * \brief This macro defines the model check property for a valid
 * bloom_filter_t structure.
 */
#define MODEL_PROP_VALID_BLOOM_FILTER(bloom) \
    (NULL != bloom && NULL != (bloom)->hdr.dispose && NULL != (bloom)->options && NULL != (bloom)->bitmap)


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
 * space required to meet the target error rate is below the specified
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
int bloom_filter_options_init(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, unsigned int num_expected_entries,
    float target_error_rate, size_t max_size_in_bytes);


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
int bloom_filter_options_init_ex(bloom_filter_options_t* options,
    allocator_options_t* alloc_opts, unsigned int num_expected_entries,
    float target_error_rate, size_t max_size_in_bytes,
    hash_func_t hash_function_1, hash_func_t hash_function_2);

/**
 * \brief Initialize a bloom filter.
 *
 * This method allows for the creation of a bloom filter.  Once initialized,
 * the filter will be empty.

 * When the function completes successfully, the caller owns this
 * ::bloom_filter_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The bloom filter options to use for this instance.
 * \param bloom             The bloom filter to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_BLOOM_BITMAP_ALLOCATION_FAILED if memory could not
 *        be allocated for the bloom filter.
 */
int bloom_filter_init(bloom_filter_options_t* options, bloom_filter_t* bloom);

/**
 * \brief Hash a null terminated input value to determine which bit of the
 * filter to set.
 *
 * \param options           The bloom filter options to use for this instance.
 * \param data              The null terminated data to hash.
 * \param n                 The round of hashing represented by this operation.
 *                          If the filter has K hash functions, this value
 *                          should be in the range [0, K).
 *
 * \returns The position of the bit in the filter to set to 1.
 */
unsigned int bloom_filter_hash(bloom_filter_options_t* options, const void* data,
    unsigned int n);

/**
 * \brief Add an item to a bloom filter.
 *
 * \param bloom             The bloom filter.
 * \param data              The null terminated data to add to the filter.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int bloom_filter_add_item(bloom_filter_t* bloom, const void* data);

/**
 * \brief Query a bloom filter to determine if an item has been added.
 *
 * \param bloom             The bloom filter.
 * \param data              The null terminated data to query the filter for.
 *
 * \returns a boolean value indicating if the data is present in the filter.
 */
_Bool bloom_filter_contains_item(bloom_filter_t* bloom, const void* data);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_BLOOM_FILTER_HEADER_GUARD
