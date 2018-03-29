/**
 * \file abstract_factory.c
 *
 * Implementation of the abstract factory.
 *
 * \copyright 2017 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdbool.h>
#include <vpr/abstract_factory.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>
#include <vpr/parameters.h>

/* this is the real implementation. */
#ifndef MODEL_CHECK_vpr_abstract_factory_shadowed

/**
 * Instantiation flag.  This flag maintains whether the abstract factory has
 * been instantiated or not.  It must be set to false.
 */
static bool abstract_factory_instantiated = false;

/**
 * Failure flag.  This flag is set if setting up the abstract factory failed,
 * and it will cause all abstract factory logic to short circuit to failure.
 */
static bool abstract_factory_failure = true;

/**
 * Allocator to use for the abstract factory.
 */
static allocator_options_t abstract_factory_alloc_options;

/**
 * This options structure contains the options details needed for working with
 * our abstract factory dynamic array.
 */
static dynamic_array_options_t abstract_factory_array_options;

/**
 * This dynamic array contains the abstract factory instances.
 */
static dynamic_array_t abstract_factory_registry;

//forward decls
static void abstract_factory_init_one_shot();
static int interface_impl_feature_compare(const void*, const void*, size_t);
static int feature_match(const void*, const void*, size_t);

/**
 * Register an implementation in the abstract factory.
 *
 * Note: this method is NOT threadsafe.  All registrations should happen before
 * any other abstract factory methods are used by other threads.
 *
 * \param impl          The factory registration structure to register.
 */
void abstract_factory_register(abstract_factory_registration_t* impl)
{
    abstract_factory_init_one_shot();

    /* the factory should now be instantiated if it wasn't before. */
    MODEL_ASSERT(abstract_factory_instantiated || abstract_factory_failure);

    /* if we are in failure mode, fail. */
    if (abstract_factory_failure)
        return;

    /* grow the registry if we need more space */
    if (abstract_factory_registry.elements ==
        abstract_factory_registry.reserved_elements)
    {
        dynamic_array_grow(&abstract_factory_registry,
            abstract_factory_registry.reserved_elements + 50);
    }

    /* register this instance */
    dynamic_array_append(&abstract_factory_registry, impl);
}

/**
 * One-shot initialization of the abstract factory.
 */
static void abstract_factory_init_one_shot()
{
    if (!abstract_factory_instantiated)
    {
        /* set us up in failure case by default */
        abstract_factory_failure = true;

        /* initialize the allocator options */
        malloc_allocator_options_init(&abstract_factory_alloc_options);

        /* initialize the array options */
        if (0 !=
            dynamic_array_options_init(
                &abstract_factory_array_options,
                &abstract_factory_alloc_options,
                sizeof(abstract_factory_registration_t),
                &interface_impl_feature_compare))
            return;

        /* initialize the array */
        if (0 !=
            dynamic_array_init(
                &abstract_factory_array_options, &abstract_factory_registry,
                50, 0, NULL))
            return;

        /* we are now instantiated */
        abstract_factory_failure = false;
        abstract_factory_instantiated = true;
    }
}

/**
 * Compare two registry entries based on their interface, implementation, and
 * feature flags.
 *
 * \param x             The left-hand element.
 * \param y             The right-hand element.
 * \param size          The size of this type.
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
static int interface_impl_feature_compare(
    const void* x, const void* y, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(y != NULL);
    MODEL_ASSERT(size == sizeof(abstract_factory_registration_t));

    abstract_factory_registration_t* rx = (abstract_factory_registration_t*)x;
    abstract_factory_registration_t* ry = (abstract_factory_registration_t*)y;

    if (rx->interface != ry->interface)
    {
        return compare_uint32(&rx->interface,
            &ry->interface,
            sizeof(uint32_t));
    }
    else if (rx->implementation != ry->implementation)
    {
        return compare_uint32(&rx->implementation,
            &ry->implementation,
            sizeof(uint32_t));
    }
    else
    {
        return compare_uint32(&rx->implementation_features,
            &ry->implementation_features,
            sizeof(uint32_t));
    }
}

/**
 * Look up an implementation of a given interface that includes the given
 * features.
 *
 * \param interface     The interface ID to look up.
 * \param features      The features this interface must have.
 *
 * \returns a matching registration or NULL if no suitable implementation was
 * found.
 */
abstract_factory_registration_t*
abstract_factory_find(uint32_t interface, uint32_t features)
{
    MODEL_ASSERT(abstract_factory_instantiated || abstract_factory_failure);

    /* this method can't be run until factories have been registered, or if
     * registration setup failed. */
    if (!abstract_factory_instantiated || abstract_factory_failure)
    {
        return NULL;
    }

    //set up our match parameters
    abstract_factory_registration_t elem;
    elem.interface = interface;
    elem.implementation = 0;
    elem.implementation_features = features;

    return dynamic_array_linear_search(
        &abstract_factory_registry, &feature_match, &elem);
}

/**
 * Compare a registered implementation against a given feature set.
 *
 * \param x             The left-hand element.
 * \param elem          The element to compare.
 * \param size          The size of this type.
 *
 * \returns (> 0 if x > y) (< 0 if x < y) (== 0 if x == y)
 */
static int feature_match(
    const void* x, const void* elem, size_t UNUSED(size))
{
    MODEL_ASSERT(x != NULL);
    MODEL_ASSERT(elem != NULL);
    MODEL_ASSERT(size == sizeof(abstract_factory_registration_t));

    abstract_factory_registration_t* rx = (abstract_factory_registration_t*)x;
    abstract_factory_registration_t* re =
        (abstract_factory_registration_t*)elem;

    if (rx->interface != re->interface)
    {
        return compare_uint32(&rx->interface,
            &re->interface,
            sizeof(uint32_t));
    }
    else if (re->implementation != 0 &&
        rx->implementation != re->implementation)
    {
        return compare_uint32(&rx->implementation,
            &re->implementation,
            sizeof(uint32_t));
    }
    else
    {
        //mask our features against the features provided by rx.  If all
        //features we've requested match, then this implementation will work.
        return re->implementation_features -
            (rx->implementation_features & re->implementation_features);
    }
}

#endif /*!defined(MODEL_CHECK_vpr_abstract_factory_shadowed)*/
