/**
 * \file error_codes.h
 *
 * \brief Error codes for vpr.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_ERROR_CODES_HEADER_GUARD
#define VPR_ERROR_CODES_HEADER_GUARD

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \defgroup VPRErrorCodes Error codes for the Velo Portable Runtime.
 *
 * \brief These status and error codes are returned by VPR functions.
 *
 * @{
 */

/**
 * \brief The VPR_STATUS_SUCCESS code represents the successful completion of a
 * VPR method.
 */
#define VPR_STATUS_SUCCESS 0x0000

/**
 * \brief This error code is returned when the reserve size for a
 * ::dynamic_array_t is exceeded during a dynamic_array_append() operation.
 */
#define VPR_ERROR_DYNAMIC_ARRAY_APPEND_NO_RESERVE 0x1100

/**
 * \brief This error code is returned by dynamic_array_init() when memory
 * allocation fails.
 */
#define VPR_ERROR_DYNAMIC_ARRAY_INIT_ALLOCATION_FAILED 0x1101

/**
 * \brief This error code is returned by dynamic_array_sort() when a general
 * error occurs.
 */
#define VPR_ERROR_DYNAMIC_ARRAY_SORT_GENERAL 0x1102

/**
 * \brief This error code is returned by dynamic_array_sort() when memory
 * allocation fails.
 */
#define VPR_ERROR_DYNAMIC_ARRAY_SORT_ALLOCATION_FAILED 0x1103

/**
 * \brief This error code is returned by dynamic_array_grow() when an invalid
 * argument is provided by the caller.
 */
#define VPR_ERROR_DYNAMIC_ARRAY_GROW_INVALID_ARGUMENT 0x1104

/**
 * \brief This error code is returned by dynamic_array_grow() when memory
 * allocation fails.
 */
#define VPR_ERROR_DYNAMIC_ARRAY_GROW_ALLOCATION_FAILED 0x1105

/**
 * \brief This error code is returned by allocator_control() when a key is
 * invalid for an underlying allocator.
 */
#define VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY 0x1106

/**
 * \brief This error code is returned by bump_allocator_options_init() when the
 * backing buffer is too small.
 */
#define VPR_ERROR_BUMP_ALLOCATOR_TINY_BUFFER 0x1107

/**
 * \brief This error code is returned by doubly_linked_list_insert_after()
 * when memory could not be allocated for a new element.
 */
#define VPR_ERROR_DLL_AFTER_ELEMENT_ALLOCATION_FAILED 0x1200

/**
 * \brief This error code is returned by doubly_linked_list_insert_after()
 * when a buffer to hold a copy of the inserted data could not be allocated.
 */
#define VPR_ERROR_DLL_AFTER_BUFFER_ALLOCATION_FAILED 0x1201

/**
 * \brief This error code is returned by doubly_linked_list_insert_before()
 * when memory could not be allocated for a new element.
 */
#define VPR_ERROR_DLL_BEFORE_ELEMENT_ALLOCATION_FAILED 0x1202

/**
 * \brief This error code is returned by doubly_linked_list_insert_before()
 * when a buffer to hold a copy of the inserted data could not be allocated.
 */
#define VPR_ERROR_DLL_BEFORE_BUFFER_ALLOCATION_FAILED 0x1203

/**
 * \brief This error code is returned by doubly_linked_list_insert_beginning()
 * when memory could not be allocated for a new element.
 */
#define VPR_ERROR_DLL_BEGINNING_ELEMENT_ALLOCATION_FAILED 0x1204

/**
 * \brief This error code is returned by doubly_linked_list_insert_beginning()
 * when a buffer to hold a copy of the inserted data could not be allocated.
 */
#define VPR_ERROR_DLL_BEGINNING_BUFFER_ALLOCATION_FAILED 0x1205


/**
 * \brief This error code is returned by bloom_filter_init() when memory could
 * not be allocated for the bloom filter.
 */
#define VPR_ERROR_BLOOM_BITMAP_ALLOCATION_FAILED 0x1300


/**
 * \brief This error code is returned by hashmap_init() when memory could not
 * be allocated to initialize the hashmap.
 */
#define VPR_ERROR_HASHMAP_ALLOCATION_FAILED 0x1400


/**
 * @}
 */

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_ERROR_CODES_HEADER_GUARD
