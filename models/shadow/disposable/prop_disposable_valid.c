/**
 * \file models/shadow/disposable/prop_disposable_valid.c
 *
 * \brief Returns true if the given disposable is valid.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

MODEL_STRUCT_TAG_GLOBAL_EXTERN(disposable);

/**
 * \brief Return true if the given disposable is valid.
 *
 * \param disp      Disposable instance to check.
 *
 * \returns true if valid and false otherwise.
 */
bool prop_disposable_valid(const disposable_t* disp)
{
    MODEL_ASSERT_STRUCT_TAG_INITIALIZED(
        disp->MODEL_STRUCT_TAG_REF(disposable), disposable);

    return
        (NULL != disp) && (NULL != disp->dispose);
}
