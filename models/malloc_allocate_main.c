#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>

uint32_t nondet_u32();

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;

    malloc_allocator_options_init(&alloc_opts);
    MODEL_ASSERT(prop_allocator_valid(&alloc_opts));

    /* call the allocator in our model check. */
    int* x = (int*)allocate(&alloc_opts, sizeof(int));
    if (NULL == x)
    {
        return 0;
    }

    /* make sure we can use this memory location. */
    *x = 17;

    /* reallocate the memory. */
    x = reallocate(&alloc_opts, x, sizeof(int), 2 * sizeof(int));

    /* we can write to both locations now. */
    x[0] = 19;
    x[1] = 20;

    /* call release in our model check. */
    release(&alloc_opts, x);

    /* the control function always returns an invalid key. */
    uint32_t key = nondet_u32();
    void* value = NULL;
    MODEL_ASSERT(
        VPR_ERROR_ALLOCATOR_CONTROL_INVALID_KEY ==
            allocator_control(&alloc_opts, key, &value));

    /* dispose the allocator. */
    dispose(allocator_options_disposable_handle(&alloc_opts));

    return 0;
}
