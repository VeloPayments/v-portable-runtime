/**
 * \file disposable.h
 *
 * \brief Disposable interface for data structures.
 *
 * When working with the disposable pattern, it is expected that the dispose
 * method for the given structure releases any resources connected with that
 * structure.  If the structure is allocated on the stack, then after dispose()
 * is called, it is safe for this structure to fall out of scope.  If the
 * structure is allocated on the heap, then after dispose() is called, it is
 * safe for this structure to be released to the heap.
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
 * \brief The dispose method takes a pointer to the underlying structure and
 * performs any resource cleanup necessary before the structure itself is
 * released.
 */
typedef void (*dispose_method_t)(void*);

/**
 * \brief The disposable interface allows a data structure to be disposed when
 * no longer used.
 *
 * C supports a HAS-A relationship for inheritance in data structures.  In any
 * structure that has a ::disposable_t header, it is possible to cast this
 * structure to the ::disposable_t type and then call dispose() on this
 * structure.
 */
typedef struct disposable
{
    /**
     * \brief The dispose method for this disposable type.
     */
    dispose_method_t dispose;
} disposable_t;

/**
 * \brief This macro defines the model check property for a valid disposable
 * structure.
 */
#define MODEL_PROP_VALID_DISPOSABLE(options) \
    (NULL != options && NULL != (options)->dispose)

/**
 * \brief Dispose of a disposable structure.
 *
 * Call its dispose method.
 *
 * \param disp      Disposable structure to be disposed.
 */
void dispose(disposable_t* disp);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //VPR_DISPOSABLE_HEADER_GUARD
