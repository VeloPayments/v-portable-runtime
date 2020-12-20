#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <string.h>
#include <vpr/allocator.h>

static void mydispose(void* m);
static void* myallocate(void* c, size_t s);
static void myrelease(void* c, void* m);
static void* myreallocate(void* c, void* m, size_t o, size_t n);

bool allocate_called = false;
bool reallocate_called = false;
bool release_called = false;
bool dispose_called = false;

int context = 7;

void* mem;
size_t size;
size_t old_size;
size_t new_size;

int main(int argc, char* argv[])
{
    allocator_options_t alloc_opts;

    alloc_opts.hdr.dispose = &mydispose;
    alloc_opts.allocator_allocate = &myallocate;
    alloc_opts.allocator_release = &myrelease;
    alloc_opts.allocator_reallocate = &myreallocate;
    alloc_opts.context = &context;

    /* preconditions for allocate. */
    allocate_called = false;
    size = 0;

    /* call allocate */
    void* ptr = allocate(&alloc_opts, 10);

    /* postconditions */
    MODEL_ASSERT(allocate_called);
    if (ptr == NULL)
        return 1;
    MODEL_ASSERT(ptr != NULL);
    MODEL_ASSERT(size == 10);

    /* preconditions for reallocate */
    reallocate_called = false;
    mem = NULL;
    old_size = new_size = 0;

    /* call reallocate */
    void* ptr2 = reallocate(&alloc_opts, ptr, 10, 15);

    /* postconditions */
    MODEL_ASSERT(reallocate_called);
    if (ptr2 == NULL)
    {
        mem = 0;
        release(&alloc_opts, ptr);
        MODEL_ASSERT(mem == ptr);

        return 1;
    }
    MODEL_ASSERT(mem == ptr);
    MODEL_ASSERT(old_size == 10);
    MODEL_ASSERT(new_size == 15);
    MODEL_ASSERT(ptr2 != ptr);


    /* preconditions for release. */
    release_called = false;
    mem = 0;

    /* release memory. */
    release(&alloc_opts, ptr2);

    /* postconditions for release. */
    MODEL_ASSERT(release_called);
    MODEL_ASSERT(mem == ptr2);

    /* preconditions for dispose */
    dispose_called = false;

    /* dispose of allocator options. */
    dispose(allocator_options_disposable_handle(&alloc_opts));

    /* postconditions for dispose. */
    MODEL_ASSERT(dispose_called);
    MODEL_ASSERT(alloc_opts.hdr.dispose == 0);
    MODEL_ASSERT(alloc_opts.allocator_allocate == 0);
    MODEL_ASSERT(alloc_opts.allocator_release == 0);
    MODEL_ASSERT(alloc_opts.allocator_reallocate == 0);
    MODEL_ASSERT(alloc_opts.context == 0);

    return 0;
}

/* test that dispose works properly and set variables accordingly. */
static void mydispose(void* m)
{
    dispose_called = true;
    //memset(m, 0, sizeof(allocator_options_t));
    allocator_options_t* alloc_opts = (allocator_options_t*)m;
    alloc_opts->hdr.dispose = NULL;
    alloc_opts->allocator_allocate = NULL;
    alloc_opts->allocator_release = NULL;
    alloc_opts->allocator_reallocate = NULL;
    alloc_opts->allocator_control = NULL;
    alloc_opts->context = NULL;
}

/* test that allocate works properly and set variables accordingly. */
static void* myallocate(void* c, size_t s)
{
    allocate_called = true;
    MODEL_ASSERT(c == &context);

    size = s;

    return malloc(s);
}

/* test that release works properly and set variables accordingly. */
static void myrelease(void* c, void* m)
{
    release_called = true;

    MODEL_ASSERT(c == &context);
    MODEL_ASSERT(m != NULL);

    mem = m;

    free(m);
}

/* We are just testing the mechanism of reallocation.  No copying. */
static void* myreallocate(void* c, void* m, size_t o, size_t n)
{
    reallocate_called = true;
    mem = m;
    old_size = o;
    new_size = n;

    void* ret = malloc(n);

    if (ret)
        free(m);

    return ret;
}
