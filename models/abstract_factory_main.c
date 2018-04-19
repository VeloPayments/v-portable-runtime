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
static abstract_factory_registration_t* lookup_foo();
static abstract_factory_registration_t* lookup_bar();

#define FOO_IMPL 7
static impl_test_t global_foo;
static abstract_factory_registration_t global_foo_impl;
#define BAR_IMPL 8
impl_test_t global_bar;
static abstract_factory_registration_t global_bar_impl;

int main(int argc, char* argv[])
{
    register_foo();
    register_bar();

    abstract_factory_registration_t* foo_reg = lookup_foo();
    abstract_factory_registration_t* bar_reg = lookup_bar();

    /* if dynamic memory allocation fails, both entries will be NULL. */
    if (NULL == foo_reg && NULL == bar_reg)
        return 0;

    /* otherwise, we should be successful. */
    MODEL_ASSERT(NULL != foo_reg);
    impl_test_t* foo_impl = (impl_test_t*)foo_reg->context;
    MODEL_ASSERT(NULL != bar_reg);
    impl_test_t* bar_impl = (impl_test_t*)bar_reg->context;

    /* clean up the abstract factory instance. */
    abstract_factory_dispose();

    return 0;
}

static void register_foo()
{
    global_foo.name = "foo";
    global_foo_impl.interface = TEST_INTERFACE;
    global_foo_impl.implementation = FOO_IMPL;
    global_foo_impl.implementation_features = FOO_IMPL;
    global_foo_impl.factory = 0;
    global_foo_impl.context = &global_foo;
    abstract_factory_register(&global_foo_impl);
}

static abstract_factory_registration_t* lookup_foo()
{
    abstract_factory_registration_t* reg = NULL;

    reg = abstract_factory_find(TEST_INTERFACE, FOO_IMPL);
    if (NULL != reg)
    {
        MODEL_ASSERT(reg->implementation == FOO_IMPL);
        return reg;
    }

    return NULL;
}

static void register_bar()
{
    global_bar.name = "bar";
    global_bar_impl.interface = TEST_INTERFACE;
    global_bar_impl.implementation = BAR_IMPL;
    global_bar_impl.implementation_features = BAR_IMPL;
    global_bar_impl.factory = 0;
    global_bar_impl.context = &global_bar;
    abstract_factory_register(&global_bar_impl);
}

static abstract_factory_registration_t* lookup_bar()
{
    abstract_factory_registration_t* reg = NULL;

    reg = abstract_factory_find(TEST_INTERFACE, BAR_IMPL);
    if (NULL != reg)
    {
        MODEL_ASSERT(reg->implementation == BAR_IMPL);
        return reg;
    }

    return NULL;
}
