/**
 * \file models/shadow/allocator/prop_allocator_valid.c
 *
 * \brief Returns true if the given allocator is valid.
 *
 * \copyright 2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>

MODEL_STRUCT_TAG_GLOBAL_EXTERN(allocator);

/**
 * \brief Return true if the given allocator is valid.
 *
 * \note this function is only available for model checking.
 *
 * \param alloc     allocator instance to check.
 *
 * \returns true if valid and false otherwise.
 */
bool prop_allocator_valid(const allocator_options_t* alloc)
{
    MODEL_ASSERT_STRUCT_TAG_INITIALIZED(
        alloc->MODEL_STRUCT_TAG_REF(allocator), allocator);

    return
           NULL != alloc
        && prop_disposable_valid(&alloc->hdr)
        && NULL != alloc->allocator_allocate
        && NULL != alloc->allocator_release
        && NULL != alloc->allocator_control;
}
