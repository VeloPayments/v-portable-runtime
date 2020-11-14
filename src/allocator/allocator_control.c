/**
 * \file allocator_control.c
 *
 * Implementation of allocator.allocator_control.
 *
 * \copyright 2018-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <vpr/allocator.h>
#include <string.h>

/**
 * \brief Make an allocator control call.
 *
 * \param options       Allocator options to use for this control call.
 * \param key           The control key being called.
 * \param value         The optional value parameter for this control cal.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY if the control key is
 *        invalid for the underlying allocator.
 *      - a non-zero error specific to a given control call on failure.
 */
int allocator_control(allocator_options_t* options, uint32_t key, void* value)
{
    MODEL_ASSERT(prop_allocator_valid(options));

    return options->allocator_control(options->context, key, value);
}
