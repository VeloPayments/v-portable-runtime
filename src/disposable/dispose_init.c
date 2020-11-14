/**
 * \file dispose_init.c
 *
 * Implementation of disposable.dispose_init.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

MODEL_STRUCT_TAG_GLOBAL_EXTERN(disposable);

/**
 * \brief Initialize a disposable instance with the given dispose method.
 *
 * \param disp      The disposable instance to initialize.
 * \param func      The dispose method to use to dispose this instance.
 */
void dispose_init(disposable_t* disp, dispose_method_t func)
{
    /* parameter sanity checks. */
    MODEL_ASSERT(NULL != disp);
    MODEL_ASSERT(NULL != func);

    /* set the dispose function. */
    disp->dispose = func;

    /* set the struct tag for model checking. */
    MODEL_STRUCT_TAG_INIT(disp->MODEL_STRUCT_TAG_REF(disposable), disposable);

    /* verify that this disposable instance is valid. */
    MODEL_ASSERT(prop_disposable_valid(disp));
}
