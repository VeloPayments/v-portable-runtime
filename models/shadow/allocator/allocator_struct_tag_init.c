/**
 * \file models/shadow/allocator/allocator_struct_tag_init.c
 *
 * \brief Initialize the allocator struct tag.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

int MODEL_STRUCT_TAG_GLOBAL_REF(allocator);

void allocator_struct_tag_init()
{
    MODEL_STRUCT_TAG_GLOBAL_INIT(allocator);
}
