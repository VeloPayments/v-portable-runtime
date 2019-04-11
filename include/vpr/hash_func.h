/**
 * \file hash_func.h
 *
 * \brief Hash function
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_HASH_FUNC_HEADER_GUARD
#define VPR_HASH_FUNC_HEADER_GUARD


/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus


/**
 * \brief A hash function that hashes an arbitrary amount of data into a
 * 64 bit value.
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
 * \param data   The data to be hashed.
 * \param len    The length of the data to be hashed.
 *
 * \returns a 64 bit hash value
 */
typedef uint64_t (*hash_func_t)(const void* data, size_t len);


/**
 * An implementation of the sdbm hash algorithm
 *
 * \param data               The data to hash
 * \param len                The length of the data to be hashed.
 *
 * \returns The 64 bit hash signature of the input data
 */
uint64_t sdbm(const void* data, size_t len);

/**
 * An implementation of the jenkins hash algorithm
 *
 * \param data               The data to hash
 * \param len                The length of the data to be hashed.
 *
 * \returns The 64 bit hash signature of the input data
 */
uint64_t jenkins(const void* data, size_t len);


/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_HASH_FUNC_HEADER_GUARD
