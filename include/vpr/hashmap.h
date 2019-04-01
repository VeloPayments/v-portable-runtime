/**
 * \file hashmap.h
 *
 * \brief Hashmap
 *
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_HASHMAP_HEADER_GUARD
#define VPR_HASHMAP_HEADER_GUARD

#include <vpr/allocator.h>
#include <vpr/disposable.h>
#include <vpr/error_codes.h>
#include <stdlib.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus


/**
 * \brief This structure contains the options used by a hashmap instance.
 *
 * User code will create an options structure using the
 * hashmap_options_init() method declared below.  When this set of options
 * is no longer required, it should be disposed using dispose().
 */
typedef struct hashmap_options
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
     * \brief The number of buckets in this hashmap.
     */
    unsigned int capacity;

} hashmap_options_t;


/**
 * \brief This macro defines the model check property for a valid
 * hashmap_options_t structure.
 */
#define MODEL_PROP_VALID_HASHMAP_OPTIONS(options) \
    (NULL != options && NULL != (options)->hdr.dispose && NULL != (options)->alloc_opts && (options)->capacity > 0)


/**
 * \brief A hashmap entry
 */
typedef struct hashmap_entry
{
    /**
     * \brief The full key for this entry.
     */
    uint64_t key;

    /**
     * \brief Opaque pointer to the value being stored.
     */
    void* val;

} hashmap_entry_t;

/**
 * \brief The hashmap structure.
 */
typedef struct hashmap
{
    /**
     * \brief This structure is disposable.
     */
    disposable_t hdr;

    /**
     * \brief The options used to create this hashmap.
     */
    hashmap_options_t* options;

    /**
     * \brief Opaque pointer to the buckets array.
     */
    void* buckets;

    /**
     * \brief The number of elements in this hashmap.
     */
    unsigned int elements;

} hashmap_t;


/**
 * \brief This macro defines the model check property for a valid
 * hashmap_t structure.
 */
#define MODEL_PROP_VALID_HASHMAP(hmap) \
    (NULL != hmap && NULL != (hmap)->hdr.dispose && NULL != (hmap)->options && NULL != (hmap)->buckets)


/**
 * \brief Initialize a hashmap
 *
 * \param options                  The hashmap options to initialize.
 * \param alloc_opts               The allocator options to use.
 * \param capacity                 The number of buckets to allocate.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    unsigned int capacity);

/**
 * \brief Initialize a hashmap
 *
 * \param options                  The hashmap options to initialize.
 * \param alloc_opts               The allocator options to use.
 * \param capacity                 The number of buckets to allocate.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init_ex(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    unsigned int capacity);


/**
 * \brief Initialize a hashmap.
 *
 * This method allows for the creation of a hashmap.  Once initialized the
 * hashmap will be empty.

 * When the function completes successfully, the caller owns this
 * ::hashmap_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The hashmap options to use for this instance.
 * \param hmap              The hashmap to initialize.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_HASHMAP_ALLOCATION_FAILED if memory could not
 *        be allocated for the hashmap
 */
int hashmap_init(hashmap_options_t* options, hashmap_t* hmap);

void* hashmap_get(hashmap_t* hmap, uint64_t key);

int hashmap_put(hashmap_t* hmap, uint64_t key, void* val);


/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_HASHMAP_HEADER_GUARD
