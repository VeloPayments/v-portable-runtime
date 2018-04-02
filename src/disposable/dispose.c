/**
 * \file dispose.c
 *
 * Implementation of disposable.dispose.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

/* this is the real implementation. */
#ifndef MODEL_CHECK_vpr_dispose_shadowed

/**
 * \brief Dispose of a disposable structure.
 *
 * Call its dispose method.
 *
 * \param disp      Disposable structure to be disposed.
 */
void dispose(disposable_t* disp)
{
    MODEL_ASSERT(MODEL_PROP_VALID_DISPOSABLE(disp));

    disp->dispose(disp);
}

#endif /*!defined(MODEL_CHECK_vpr_dispose_shadowed)*/
