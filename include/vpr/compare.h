/**
 * \file compare.h
 *
 * \brief Some pre-defined comparison methods and a standard function pointer
 * type for comparison methods.
 *
 * The purpose of these methods is to provide a generic comparison function
 * pointer type for container types that either require sorting or provide the
 * ability to be sorted, such as \ref dynamic_array_t.  These sort methods are
 * link-time optimized, meaning that if the static library is used, only the
 * sort methods that are used by the application will be linked to the
 * application.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_COMPARE_HEADER_GUARD
#define VPR_COMPARE_HEADER_GUARD

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * \defgroup ComparisonResults Comparison constants for VPR.
 *
 * \brief These constants are returned by a subset of the comparison functions
 * below.
 *
 * It is expected that application code compare against zero for proper
 * functionality.  Less-than is x < 0.  Greater-than is x > 0.  Equal-to is x ==
 * 0.
 * @{
 */

/**
 * \brief lhs > rhs.
 */
#define VPR_COMPARE_GREATER 1

/**
 * \brief lhs < rhs.
 */
#define VPR_COMPARE_LESS -1

/**
 * \brief lhs == rhs.
 */
#define VPR_COMPARE_EQUAL 0
/**
 * @}
 */

/**
 * \brief The comparison method to use when comparing two elements in this array.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (generally ignored for builtins).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
typedef int (*compare_method_t)(const void* x, const void* y, size_t size);

/**
 * \brief Compare two signed characters.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_char(const void* x, const void* y, size_t size);

/**
 * \brief Compare two unsigned characters.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_char(const void* x, const void* y, size_t size);

/**
 * \brief Compare two signed shorts.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_short(const void* x, const void* y, size_t size);

/**
 * \brief Compare two unsigned shorts.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_short(const void* x, const void* y, size_t size);

/**
 * \brief Compare two signed ints.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_int(const void* x, const void* y, size_t size);

/**
 * \brief Compare two unsigned ints.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_int(const void* x, const void* y, size_t size);

/**
 * \brief Compare two signed longs.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_long(const void* x, const void* y, size_t size);

/**
 * \brief Compare two unsigned longs.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_long(const void* x, const void* y, size_t size);

/**
 * \brief Compare two signed long longs.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_long_long(const void* x, const void* y, size_t size);

/**
 * \brief Compare two unsigned long longs.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_unsigned_long_long(const void* x, const void* y, size_t size);

/**
 * \brief Compare two floats.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_float(const void* x, const void* y, size_t size);

/**
 * \brief Compare two doubles.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_double(const void* x, const void* y, size_t size);

/**
 * \brief Compare two long doubles.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_long_double(const void* x, const void* y, size_t size);

/**
 * \brief Compare two int8_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_int8(const void* x, const void* y, size_t size);

/**
 * \brief Compare two uint8_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_uint8(const void* x, const void* y, size_t size);

/**
 * \brief Compare two int16_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_int16(const void* x, const void* y, size_t size);

/**
 * \brief Compare two uint16_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_uint16(const void* x, const void* y, size_t size);

/**
 * \brief Compare two int32_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_int32(const void* x, const void* y, size_t size);

/**
 * \brief Compare two uint32_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_uint32(const void* x, const void* y, size_t size);

/**
 * \brief Compare two int64_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_int64(const void* x, const void* y, size_t size);

/**
 * \brief Compare two uint64_t values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_uint64(const void* x, const void* y, size_t size);

/**
 * \brief Compare two bool values.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type (ignored).
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
int compare_bool(const void* x, const void* y, size_t size);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_COMPARE_HEADER_GUARD
