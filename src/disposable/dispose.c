/**
 * \file dispose.c
 *
 * Implementation of disposable.dispose.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

/**
 * Dispose of a disposable structure.  Call its dispose method.
 *
 * \param disp      Disposable structure to be disposed.
 */
void dispose(disposable_t* disp)
{
    MODEL_ASSERT(disp != NULL);
    MODEL_ASSERT(disp->dispose != NULL);

    disp->dispose(disp);
}
