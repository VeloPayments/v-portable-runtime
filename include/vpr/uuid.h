/**
 * \file uuid.h
 *
 * \brief This header provides a simple uuid data structure as well as function
 * declarations for working with a uuid.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_UUID_HEADER_GUARD
#define VPR_UUID_HEADER_GUARD

#include <stdint.h>
#include <vpr/allocator.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* forward decl for uuid. */
typedef struct vpr_uuid
    vpr_uuid;

/**
 * \brief The UUID is a structure with size, representing a primitive UUID
 * value.
 */
struct vpr_uuid
{
    uint8_t data[16];
};

/**
 * \brief Convert a uuid value to a string.
 *
 * \note On success, the char pointer pointer is updated to point to a string
 * value allocated by this function.  This string value is owned by the caller
 * and must be released when no longer needed.
 *
 * \param str           Pointer to the pointer to receive the converted string.
 * \param alloc_opts    The allocator options to use to allocate this string
 *                      value.
 * \param uuid          The \ref vpr_uuid value to convert to a string.
 *
 * \returns a status code indicating success or failure.
 *      - VPR_STATUS_SUCCESS on success.
 *      - VPR_ERROR_UUID_ALLOCATION_FAILED if memory allocation for the string
 *        failed.
 */
int vpr_uuid_to_string(
    char** str, allocator_options_t* alloc_opts, const vpr_uuid* uuid);

/**
 * \brief Convert a string to a uuid value.
 *
 * \note On success, the uuid pointer is updated to reflect the value of the
 * given string.
 *
 * \param uuid          The \ref vpr_uuid value to convert to a string.
 * \param str           Pointer to the pointer to receive the converted string.
 *
 * \returns a status code indicating success or failure.
 *      - VPR_STATUS_SUCCESS on success.
 *      - VPR_ERROR_UUID_CONVERSION_FAILED if the conversion to a uuid value
 *        failed.
 */
int vpr_uuid_from_string(
    vpr_uuid* uuid, const char* str);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*VPR_UUID_HEADER_GUARD*/
