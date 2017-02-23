/**
 * \file disposable.h
 *
 * Disposable interface for data structures.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#ifndef VPR_DISPOSABLE_HEADER_GUARD
#define VPR_DISPOSABLE_HEADER_GUARD

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/**
 * A dispose method takes a void pointer to the underlying structure and
 * performs any resource cleanup necessary before the structure is released.
 */
typedef void (*dispose_method_t)(void*);

/**
 * The disposable interface allows a data structure to be disposed when no
 * longer used.
 */
typedef struct disposable
{
    dispose_method_t dispose;
} disposable_t;

/**
 * Dispose of a disposable structure.  Call its dispose method.
 *
 * \param disp      Disposable structure to be disposed.
 */
void dispose(disposable_t* disp);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_DISPOSABLE_HEADER_GUARD
