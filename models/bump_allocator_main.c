#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/bump_allocator.h>

uint8_t backing_buffer[16384];

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;

    MODEL_ASSERT(
        VPR_STATUS_SUCCESS == 
        bump_allocator_options_init(
            &alloc_opts, backing_buffer, sizeof(backing_buffer)));

    MODEL_ASSERT(prop_allocator_valid(&alloc_opts));

    /* call the allocator in our model check. */
    int* x = (int*)allocate(&alloc_opts, sizeof(int));
    if (NULL == x)
    {
        return 0;
    }

    /* make sure we can use this memory location. */
    *x = 17;

    /* call release in our model check. */
    release(&alloc_opts, x);

    /* dispose the allocator. */
    dispose((disposable_t*)&alloc_opts);

    return 0;
}
