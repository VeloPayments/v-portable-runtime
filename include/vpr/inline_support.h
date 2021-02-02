/**
 * \file inline_support.h
 *
 * \brief Provide inline support through macros.
 *
 * Debug mode turns off inlining, and in that case, it must be possible to grab
 * the definitions for inline functions.
 *
 * \copyright 2021 Velo Payments, Inc.  All rights reserved.
 */

/* At the beginning of this header, remove previous macro definitions. */
#undef VPR_INLINE
#undef VPR_INLINE_DEFINITION

/* The concrete definition is defined in the concrete_impl file for a module.
 * In that case, inline functions become concrete.
 */
#if defined(VPR_CONCRETE_IMPLEMENTATION)
# define VPR_INLINE
# define VPR_INLINE_DEFINITION(x) x

/* Otherwise, in debug mode, inline is meaningless, so fall back to decls;
 * The definitions will be resolved at link time, using the concrete_impl file
 * for each module.
 */
#elif defined(DEBUG)
# define VPR_INLINE
# define VPR_INLINE_DEFINITION(x) ;

/* In release mode, the compiler will inline functions. */
#else
# define VPR_INLINE inline
# define VPR_INLINE_DEFINITION(x) x
#endif
