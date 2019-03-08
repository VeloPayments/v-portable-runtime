/**
 * \file bump_allocator_options_init_shadow.c
 *
 * Shadow Library implementation of the bump allocator.
 *
 * \copyright 2018 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/bump_allocator.h>
#include <vpr/parameters.h>

/* this is the shadow implementation. */
#ifdef MODEL_CHECK_vpr_bump_allocator_shadowed

/**
 * \brief Bump allocator context.
 */
typedef struct bump_allocator_ctx
{
    size_t offset;
    size_t max_size;
    uint8_t* arena;
} bump_allocator_ctx_t;

/**
 * \brief Constant value representing the minimum size needed for a backing
 * buffer.
 */
const size_t bump_allocator_minimum_buffer_size = sizeof(bump_allocator_ctx_t);

/**
 * \brief Dispose of our bump allocator instance.
 *
 * \param options       The options structure to free.
 */
static void bump_allocator_options_dispose(allocator_options_t* options)
{
    /* get the bump allocator context. */
    bump_allocator_ctx_t* ctx = (bump_allocator_ctx_t*)options->context;

    /* clear memory. */
    memset(ctx->arena, 0, ctx->max_size);
}

/**
 * \brief Bump allocate memory from the bump buffer.
 *
 * \param context           The user context for this operation.
 * \param size              The size to allocate.
 *
 * \returns a pointer to allocated memory on success, and NULL ond failure.
 */
static void* bump_allocate(void* context, size_t size)
{
    /* get the bump allocator context. */
    bump_allocator_ctx_t* ctx = (bump_allocator_ctx_t*)context;

    /* calculate a 128-bit aligned offset. */
    size_t mem_offset = ctx->offset;
    size_t raw_mem_offset = (size_t)(ctx->arena + mem_offset);
    if (0 != raw_mem_offset % 16)
    {
        /* increment to the next 128-bit offset. */
        mem_offset += 16 - (raw_mem_offset % 16);
    }

    /* verify that this computed offset does not exceed the total size of this
     * arena. */
    size_t bump = mem_offset + size;
    if (bump >= ctx->max_size)
    {
        return NULL;
    }

    /* bump the allocator. */
    ctx->offset += bump;

    /* return the allocated pointer. */
    return (ctx->arena + mem_offset);
}

/**
 * \brief Dummy release method.
 *
 * \param context           The user context for this operation.
 * \param ptr               The pointer to ignore / not release.
 */
static void bump_dummy_release(void* UNUSED(context), void* UNUSED(ptr))
{
    /* we can't release memory in a bump allocator. */
}

/**
 * \brief Make an allocator control call.
 *
 * \param context       User-defined context for this call.
 * \param key           The control key being called.
 * \param value         The optional value parameter for this control cal.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS if successful.
 *      - \ref VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY if the control key is
 *        invalid for the underlying allocator.
 *      - a non-zero error specific to a given control call on failure.
 */
static int bump_control(void* context, uint32_t key, void* UNUSED(value))
{
    /* get the bump allocator context. */
    bump_allocator_ctx_t* ctx = (bump_allocator_ctx_t*)context;

    switch (key)
    {
        /* reset the bump allocator for the next run. */
        case ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET:
            ctx->offset = 0;
            return VPR_STATUS_SUCCESS;

        /* invalid control key for this allocator. */
        default:
            return VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY;
    }
}

/**
 * \brief This method initializes the provided allocator_options_t structure so
 * that it is backed by a user-defined buffer of a given size.
 *
 * After bump_allocator_options_init() is called, the caller owns the
 * ::allocator_options_t structure and must dispose of it by calling dispose().
 * Furthermore, the caller RETAINS ownership of the backing buffer.  The user
 * cannot use this backing buffer until this allocator is disposed, and after it
 * is disposed, the user can release this buffer as appropriate.
 *
 * \param options       The allocation options structure to initialize.  Owned
 *                      by the caller.
 * \param buffer        The backing buffer used by this bump allocator to
 *                      allocate memory.
 * \param size          The size of the backing buffer.
 *
 * \returns a status code indicating success or failure.
 *      - \ref VPR_STATUS_SUCCESS on success.
 *      - \ref VPR_ERROR_BUMP_ALLOCATOR_TINY_BUFFER if the buffer is too small
 *        for the internal record keeping needed.
 *      - a non-zero error code indicating failure.
 */
int bump_allocator_options_init(allocator_options_t* options, void* buffer,
    size_t size)
{
    bump_allocator_ctx_t* ctx;

    /* model check parameters. */
    MODEL_ASSERT(NULL != options);
    MODEL_ASSERT(NULL != buffer);
    MODEL_ASSERT(size >= sizeof(bump_allocator_ctx_t));
    MODEL_ASSERT(((size_t)buffer) % 8 == 0);

    /* verify that the size is at least large enough to hold our context
     * structure. */
    if (size < sizeof(bump_allocator_ctx_t))
    {
        return VPR_ERROR_BUMP_ALLOCATOR_TINY_BUFFER;
    }

    /* set up our context structure. */
    ctx = (bump_allocator_ctx_t*)buffer;
    ctx->arena = ((uint8_t*)buffer) + sizeof(bump_allocator_ctx_t);
    ctx->offset = 0;
    ctx->max_size = size - sizeof(bump_allocator_ctx_t);

    /* use our internal dispose method for disposing of this structure. */
    options->hdr.dispose = (dispose_method_t)&bump_allocator_options_dispose;

    /* use our bump allocator to allocate. */
    options->allocator_allocate = &bump_allocate;

    /* use a dummy release method. */
    options->allocator_release = &bump_dummy_release;

    /* we don't support realloc. */
    options->allocator_reallocate = NULL;

    /* use our custom allocator control. */
    options->allocator_control = &bump_control;

    /* use our allocated context. */
    options->context = ctx;

    /* the allocator options structure should now be valid. */
    MODEL_ASSERT(MODEL_PROP_VALID_ALLOCATOR_OPTIONS(options));

    /* context should be set. */
    MODEL_ASSERT(options->context == ctx);

    /* success */
    return VPR_STATUS_SUCCESS;
}

#endif /*defined(MODEL_CHECK_vpr_bump_allocator_shadowed)*/
