#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <string.h>
#include <vpr/allocator.h>
#include <vpr/allocator/malloc_allocator.h>

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

    /* call release in our model check. */
    release(&alloc_opts, x);

    /* dispose the allocator. */
    dispose((disposable_t*)&alloc_opts);

    return 0;
}
