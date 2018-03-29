#include <cbmc/model_assert.h>
#include <string.h>
#include <vpr/abstract_factory.h>

#define TEST_INTERFACE 71
typedef struct impl_test
{
    const char* name;
} impl_test_t;

static void register_foo();
static void register_bar();
static impl_test_t* lookup_foo();
static impl_test_t* lookup_bar();

int main(int argc, char* argv[])
{
    register_foo();
    register_bar();

    impl_test_t* foo_impl = lookup_foo();
    impl_test_t* bar_impl = lookup_bar();

    /* if setup of the abstract factory failed, then both foo and bar impls will
     * be bad. */
    if (NULL == foo_impl && NULL == bar_impl)
        return 0;

    /* otherwise, we should be successful. */
    MODEL_ASSERT(NULL != foo_impl);
    MODEL_ASSERT(!strncmp(foo_impl->name, "foo", 4));
    MODEL_ASSERT(NULL != bar_impl);
    MODEL_ASSERT(!strncmp(bar_impl->name, "bar", 4));

    return 0;
}

#define FOO_IMPL 7
static impl_test_t global_foo;
static abstract_factory_registration_t global_foo_impl;

static void register_foo()
{
    global_foo.name = "foo";
    global_foo_impl.interface = TEST_INTERFACE;
    global_foo_impl.implementation = FOO_IMPL;
    global_foo_impl.implementation_features = FOO_IMPL;
    global_foo_impl.factory = 0;
    global_foo_impl.context = &global_foo;
}

static impl_test_t* lookup_foo()
{
    abstract_factory_registration_t* reg = NULL;

    reg = abstract_factory_find(TEST_INTERFACE, FOO_IMPL);
    if (NULL != reg)
    {
        return reg->context;
    }

    return NULL;
}

#define BAR_IMPL 8
impl_test_t global_bar;
static abstract_factory_registration_t global_bar_impl;

static void register_bar()
{
    global_bar.name = "bar";
    global_bar_impl.interface = TEST_INTERFACE;
    global_bar_impl.implementation = BAR_IMPL;
    global_bar_impl.implementation_features = BAR_IMPL;
    global_bar_impl.factory = 0;
    global_bar_impl.context = &global_bar;
}

static impl_test_t* lookup_bar()
{
    abstract_factory_registration_t* reg = NULL;

    reg = abstract_factory_find(TEST_INTERFACE, BAR_IMPL);
    if (NULL != reg)
    {
        return reg->context;
    }

    return NULL;
}
