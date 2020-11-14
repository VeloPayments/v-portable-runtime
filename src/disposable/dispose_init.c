/**
 * \file dispose_init.c
 *
 * Implementation of disposable.dispose_init.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

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

    disp->dispose = func;
}
