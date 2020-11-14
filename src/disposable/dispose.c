/**
 * \file dispose.c
 *
 * Implementation of disposable.dispose.
 *
 * \copyright 2017-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

MODEL_STRUCT_TAG_GLOBAL_EXTERN(disposable);

/**
 * \brief Dispose of a disposable structure.
 *
 * Call its dispose method.
 *
 * \param disp      Disposable structure to be disposed.
 */
void dispose(disposable_t* disp)
{
    MODEL_ASSERT(prop_disposable_valid(disp));

    disp->dispose(disp);
}
