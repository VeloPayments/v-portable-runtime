#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/bump_allocator.h>

uint8_t backing_buffer[16384];

uint32_t nondet_u32();

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;

    MODEL_ASSERT(
        VPR_STATUS_SUCCESS == 
        bump_allocator_options_init(
            &alloc_opts, backing_buffer, sizeof(backing_buffer)));

    MODEL_ASSERT(prop_allocator_valid(&alloc_opts));

    /* any key other than the RESET key fails. */
    uint32_t key = nondet_u32();
    MODEL_ASSUME(key != ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET);
    MODEL_ASSERT(
        VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY
            == allocator_control(&alloc_opts, key, NULL));

    /* we can call a reset control to reset the pointer. */
    MODEL_ASSERT(
        VPR_STATUS_SUCCESS
            == allocator_control(
                    &alloc_opts, ALLOCATOR_CONTROL_KEY_BUMP_ALLOCATOR_RESET,
                    NULL));

    /* call the allocator in our model check. */
    int* x = (int*)allocate(&alloc_opts, sizeof(int));
    if (NULL == x)
    {
        return 0;
    }

    /* make sure we can use this memory location. */
    *x = 17;

    /* we can reallocate the x pointer. */
    x = reallocate(&alloc_opts, x, sizeof(int), 2 * sizeof(int));
    MODEL_ASSERT(NULL != x);

    /* we can write to the x addresses. */
    x[0] = 1;
    x[1] = 2;

    /* call release in our model check. */
    release(&alloc_opts, x);

    /* dispose the allocator. */
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
