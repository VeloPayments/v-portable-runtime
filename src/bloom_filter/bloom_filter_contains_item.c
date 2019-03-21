/**
 * \file bloom_filter_contains_item.c
 *
 * Implementation of bloom_filter_contains_item.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <cbmc/model_assert.h>
#include <vpr/bloom_filter.h>
#include <vpr/parameters.h>

_Bool bloom_filter_contains_item(bloom_filter_t* UNUSED(bloom), const void* UNUSED(data))
{
    MODEL_ASSERT(NULL != bloom);
    MODEL_ASSERT(NULL != data);


    return VPR_STATUS_SUCCESS;
}
