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
#include <stdbool.h>
#include <stdlib.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
* \brief The copy method to use when adding items to this hashmap.
*
* \param destination   The destination to which this value will be copied.
* \param source        The source data to be copied.
* \param size          The size of the data being copied.
*/
typedef void (*hashmap_item_copy_t)(
    void* destination, const void* source, size_t size);

/**
 * \brief The dispose method to use when disposing of an item in this hashmap.
 *
 * \param alloc_opts    The allocator options to use.
 * \param item          The item to be disposed of.
 */
typedef void (*hashmap_item_dispose_t)(
    allocator_options_t* alloc_opts, void* item);

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
     * \brief The size of items added to this hashmap.
     */
    size_t item_size;

    /**
     * \brief The number of buckets in this hashmap.
     */
    unsigned int capacity;

    /**
    * \brief The copy method to use when adding items to this hashmap.
    *
    * \param destination   The destination to which this value will be copied.
    * \param source        The source data to be copied.
    * \param size          The size of the data being copied.
    */
    hashmap_item_copy_t hashmap_item_copy;

    /**
     * \brief The dispose method to use when disposing of an item in this hashmap.
     *
     * \param alloc_opts    The allocator options to use.
     * \param item          The item to be disposed of.
     */
    hashmap_item_dispose_t hashmap_item_dispose;

} hashmap_options_t;


/**
 * \brief This macro defines the model check property for a valid
 * hashmap_options_t structure.
 */
#define MODEL_PROP_VALID_HASHMAP_OPTIONS(options, sz) \
    (NULL != options && NULL != (options)->hdr.dispose && NULL != (options)->alloc_opts && (options)->item_size == sz && (NULL == (options)->hashmap_item_copy || sz > 0))


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
 * \brief Initialize hashmap options for a POD data type.
 *
 * When the function completes successfully, the caller owns this
 * ::hashmap_options_t instance and must dispose of it by calling
 * dispose() when it is no longer needed.
 *
 * \param options             The hashmap options to initialize.
 * \param alloc_opts          The allocator options to use.
 * \param capacity            The number of buckets to allocate.
 * \param copy_on_insert      If true, data will be copied before adding to the
 *                            hashmap, leaving the caller with ownership of
 *                            the original data.  The hashmap assumes
 *                            ownership of the copied data and will free it
 *                            when disposed of.
 *                            If false, the release_on_dispose argument
 *                            dictates what happens when the hashmap is
 *                            disposed of.
 * \param item_size           The size in bytes of an individual data item.
 *                            This parameter is ignored when copy_on_insert is
 *                            false, but must be a positive integer value if
 *                            copy_on_insert is true.
 * \param release_on_dispose  This parameter is ignored if copy_on_insert is
 *                            true.  When copy_on_insert is false, this
 *                            argument determines whether memory storing data
 *                            items is released when the hashmap is disposed
 *                            of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    unsigned int capacity, _Bool copy_on_insert, size_t item_size,
    _Bool release_on_dispose);

/**
 * \brief Initialize hashmap options for a custom data type.
 *
 * This method allows the user to override the copy and dispose methods to
 * manage non-POD data types and complex data structures.
 *
 * When the function completes successfully, the caller owns this
 * ::hashmap_options_t instance and must dispose of it by calling dispose()
 * when it is no longer needed.
 *
 * \param options           The hashmap options to initialize.
 * \param alloc_opts        The allocator options to use.
 * \param capacity          The number of buckets to allocate.
 * \param copy_method       Optional - The method to use to copy elements.
 *                          If provided then elements are copied into
 *                          separate memory as they are added to the list.
 * \param item_size         Optional (when copy_method is NULL).
 *                          The size of an individual data item.
 * \param dispose_method    Optional - The method to use to dispose of data
 *                          items.
 *                          If provided then this method is invoked on each
 *                          data item when the list is disposed of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init_ex(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    unsigned int capacity, hashmap_item_copy_t copy_method,
    size_t item_size, hashmap_item_dispose_t dispose_method);


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
