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
#include <vpr/hash_func.h>
#include <vpr/error_codes.h>
#include <stdbool.h>
#include <stdlib.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus


/**
 * \brief The method to test equality of two values in this hashmap.
 *
 * \param lhs         The left-hand element
 * \param rhs         The right-hand element.
 *
 * \returns true if the values are equal, otherwise false.
 */
typedef bool (*hashmap_value_equals_t)(
    const void* lhs, const void* rhs);


/**
* \brief The copy method to use when adding values to this hashmap.
*
* \param destination   The destination to which this value will be copied.
* \param source        The source data to be copied.
* \param size          The size of the value being copied.
*/
typedef void (*hashmap_value_copy_t)(
    void* destination, const void* source, size_t size);

/**
 * \brief The dispose method to use when disposing of a value in this hashmap.
 *
 * \param alloc_opts    The allocator options to use.
 * \param val           The value to be disposed of.
 */
typedef void (*hashmap_value_dispose_t)(
    allocator_options_t* alloc_opts, void* val);

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
     * \brief The size of values added to this hashmap.
     */
    size_t val_size;

    /**
     * \brief The number of buckets in this hashmap.
     */
    uint32_t capacity;

    /**
     * \brief The hash function to convert keys to a uint64_t.
     */
    hash_func_t hash_func;

    /**
     * \brief The method to test equality of two values in this hashmap.
     *
     * \param lhs         The left-hand element
     * \param rhs         The right-hand element.
     *
     * \returns true if the items are equal, otherwise false.
     */
    hashmap_value_equals_t equals_func;

    /**
    * \brief The copy method to use when adding values to this hashmap.
    *
    * \param destination   The destination to which this value will be copied.
    * \param source        The source data to be copied.
    * \param size          The size of the value being copied.
    */
    hashmap_value_copy_t copy_method;

    /**
     * \brief The dispose method to use when disposing of a value in this hashmap.
     *
     * \param alloc_opts    The allocator options to use.
     * \param val           The value to be disposed of.
     */
    hashmap_value_dispose_t dispose_method;

} hashmap_options_t;


/**
 * \brief This macro defines the model check property for a valid
 * hashmap_options_t structure.
 */
#define MODEL_PROP_VALID_HASHMAP_OPTIONS(options, sz) \
    (NULL != options && NULL != (options)->hdr.dispose && NULL != (options)->alloc_opts && (options)->val_size == sz && (NULL == (options)->copy_method || sz > 0))


/**
 * \brief A hashmap entry
 */
typedef struct hashmap_entry
{
    /**
     * \brief The hashed key for this entry.
     */
    uint64_t hashed_key;

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
    uint32_t elements;

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
 * \param equals_func         Optional - The function to test equality of two
 *                            values.  If not supplied, values are considered
 *                            equal if their hashed keys are equal.
 * \param copy_on_put         If true, values will be copied before adding to
 *                            the hashmap, leaving the caller with ownership of
 *                            the original value.  The hashmap assumes
 *                            ownership of the copied value and will free it
 *                            when disposed of.
 *                            If false, the release_on_dispose argument
 *                            dictates what happens when the hashmap is
 *                            disposed of.
 * \param val_size            The size in bytes of a value.  This parameter is
 *                            ignored when copy_on_put is false, but must be a
 *                            positive integer value if copy_on_put is true.
 * \param release_on_dispose  This parameter is ignored if copy_on_put is
 *                            true.  When copy_on_put is false, this
 *                            argument determines whether memory storing values
 *                            is released when the hashmap is disposed of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    uint32_t capacity, hashmap_value_equals_t equals_func,
    bool copy_on_put, size_t val_size, bool release_on_dispose);

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
 * \param hash_func         The hash function to use to convert variable
 *                          length keys to 64 bit keys.
 * \param equals_func       The function to test equality of two values.
 * \param copy_method       Optional - The method to use to copy values.
 *                          If provided then values are copied into separate
 *                          memory as they are added to the map.
 * \param val_size          Optional (when copy_method is NULL). The size of a
 *                          value.
 * \param dispose_method    Optional - The method to use to dispose of values.
 *                          If provided then this method is invoked on each
 *                          data item when the list is disposed of.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 */
int hashmap_options_init_ex(
    hashmap_options_t* options, allocator_options_t* alloc_opts,
    uint32_t capacity, hash_func_t hash_func,
    hashmap_value_equals_t equals_func, hashmap_value_copy_t copy_method,
    size_t val_size, hashmap_value_dispose_t dispose_method);

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

/**
 * \brief Retrieve a value from a hashmap using a variable length key.
 *
 * \param hmap              The hashmap to query
 * \param key               The key identifying the item.
 * \param key_len           The length of the key in bytes.
 *
 * \returns an opaque pointer to the item, or NULL if it wasn't found.
 */
void* hashmap_get(hashmap_t* hmap, uint8_t* key, size_t key_len);

/**
 * \brief Retrieve a value from a hashmap using a 64 bit key.
 *
 * \param hmap              The hashmap to query
 * \param key               The 64 bit key
 *
 * \returns an opaque pointer to the value, or NULL if it wasn't found.
 */
void* hashmap_get64(hashmap_t* hmap, uint64_t key);

/**
 * \brief Add a value to a hashmap.
 *
 * Add a value to the hashmap using a variable length key.
 *
 * \param hmap              The hashmap to add the value to.
 * \param key               A unique key that serves as an identifier for the
 *                          value.
 * \param key_len           The length of the key.
 * \param val               Opaque pointer to the value.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - non-zero code on failure.
 */
int hashmap_put(hashmap_t* hmap, uint8_t* key, size_t key_len, void* val);

/**
 * \brief Add a value to a hashmap using a 64 bit key.
 *
 * Add a data item to the hashmap.  The 64 bit key should be a random value
 * to minimize chaining, which will decrease performance.
 *
 * \param hmap              The hashmap to add the value to.
 * \param key               A unique key that serves as an identifier for the
 *                          value.
 * \param val               Opaque pointer to the value.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - non-zero code on failure.
 */
int hashmap_put64(hashmap_t* hmap, uint64_t key, void* val);


/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_HASHMAP_HEADER_GUARD
