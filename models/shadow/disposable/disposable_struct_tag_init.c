/**
 * \file models/shadow/disposable/disposable_struct_tag_init.c
 *
 * \brief Initialize the disposable struct tag.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/disposable.h>

int MODEL_STRUCT_TAG_GLOBAL_REF(disposable);

void disposable_struct_tag_init()
{
    MODEL_STRUCT_TAG_GLOBAL_INIT(disposable);
}
