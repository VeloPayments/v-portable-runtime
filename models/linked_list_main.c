/**
 * \file linked_list_main.c
 *
 * Simple model check of linked list
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/linked_list.h>

static void verify_empty_list();
static void verify_singleton_list();
static void verify_beginning_before();
static void verify_after_end_remove();

/**
 * Non-deterministic boolean value, provided by the model checker.
 *
 * \returns true or false in a nondeterministic manner.
 */
_Bool nondet_bool();


int main(int argc, char* argv[])
{
    verify_empty_list();
    verify_singleton_list();
    verify_beginning_before();
    verify_after_end_remove();

    return 0;
}

static void verify_empty_list()
{
    allocator_options_t alloc_opts;
    linked_list_options_t options;
    linked_list_t ll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize linked list options
    linked_list_options_init(
        &options, &alloc_opts, false, sizeof(int), false);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_LL_OPTIONS(&options, sizeof(int)));

    //initialize the doubly linked list
    MODEL_ASSERT(0 == linked_list_init(&options, &ll));

    // verify the (empty) list is valid
    MODEL_ASSERT(MODEL_PROP_VALID_LL(&ll));
    MODEL_ASSERT(0 == ll.elements);
    MODEL_ASSERT(NULL == ll.first);
    MODEL_ASSERT(NULL == ll.last);

    //dispose of the linked list
    dispose(linked_list_disposable_handle(&ll));

    //dispose of options
    dispose(linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

static void verify_singleton_list()
{
    allocator_options_t alloc_opts;
    linked_list_options_t options;
    linked_list_t ll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    // initialize linked list options
    // note: if we do copy_on_insert the last argument (release_on_dispose) is
    // ignored.  If we don't do copy_on_insert, we don't want to
    // release_on_dispose either as the data element is in stack memory.
    _Bool copy_on_insert = nondet_bool();
    linked_list_options_init(
        &options, &alloc_opts, copy_on_insert, sizeof(int), copy_on_insert);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_LL_OPTIONS(&options, sizeof(int)));

    //initialize the list
    MODEL_ASSERT(0 == linked_list_init(&options, &ll));

    // add an item to the list
    int data = 42;
    if (0 == linked_list_insert_end(&ll, &data))
    {
        // verify the singleton list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_LL(&ll));
        MODEL_ASSERT(1 == ll.elements);
        MODEL_ASSERT(NULL != ll.first);
        MODEL_ASSERT(NULL != ll.last);
        MODEL_ASSERT(NULL != ll.first->data);
        MODEL_ASSERT(*(int*)(ll.first->data) == data);
    }

    //dispose of the linked list
    dispose(linked_list_disposable_handle(&ll));

    //dispose of options
    dispose(linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

static void verify_beginning_before()
{
    allocator_options_t alloc_opts;
    linked_list_options_t options;
    linked_list_t ll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    // initialize linked list options
    // note: if we do copy_on_insert the last argument (release_on_dispose) is
    // ignored.  If we don't do copy_on_insert, we don't want to
    // release_on_dispose either as the data element is in stack memory.
    _Bool copy_on_insert = nondet_bool();
    linked_list_options_init(
        &options, &alloc_opts, copy_on_insert, sizeof(long),
        copy_on_insert);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_LL_OPTIONS(&options, sizeof(long)));

    //initialize the list
    MODEL_ASSERT(0 == linked_list_init(&options, &ll));

    long data_elements[] = { 0, 1 };

    if (0 == linked_list_insert_beginning(&ll, &data_elements[0]) && 0 == linked_list_insert_before(&ll, ll.last, &data_elements[1]))
    {
        // verify the list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_LL(&ll));
        MODEL_ASSERT(2 == ll.elements);
        MODEL_ASSERT(NULL != ll.first);
        MODEL_ASSERT(NULL != ll.last);

        // verify data
        MODEL_ASSERT(*(long*)(ll.first->data) == data_elements[1]);
        MODEL_ASSERT(*(long*)(ll.last->data) == data_elements[0]);
    }

    //dispose of the linked list
    dispose(linked_list_disposable_handle(&ll));

    //dispose of options
    dispose(linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

static void verify_after_end_remove()
{
    allocator_options_t alloc_opts;
    linked_list_options_t options;
    linked_list_t ll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    // initialize linked list options
    // note: if we do copy_on_insert the last argument (release_on_dispose) is
    // ignored.  If we don't do copy_on_insert, we don't want to
    // release_on_dispose either as the data element is in stack memory.
    _Bool copy_on_insert = nondet_bool();
    linked_list_options_init(
        &options, &alloc_opts, copy_on_insert, sizeof(long),
        copy_on_insert);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_LL_OPTIONS(&options, sizeof(long)));

    //initialize the list
    MODEL_ASSERT(0 == linked_list_init(&options, &ll));

    long data_elements[] = { 2, 3 };

    if (0 == linked_list_insert_end(&ll, &data_elements[0]) && 0 == linked_list_insert_after(&ll, ll.last, &data_elements[1]))
    {
        linked_list_element_t* elem = ll.first->next;
        linked_list_remove(&ll, elem);
        // we now "own" the element we removed
        if (copy_on_insert)
        {
            release(&alloc_opts, elem->data);
        }
        release(&alloc_opts, elem);

        // verify the list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_LL(&ll));
        MODEL_ASSERT(1 == ll.elements);
        MODEL_ASSERT(NULL != ll.first);
        MODEL_ASSERT(NULL != ll.last);
        MODEL_ASSERT(ll.first == ll.last);

        // verify pointer
        MODEL_ASSERT(NULL == ll.first->next);

        // verify data
        MODEL_ASSERT(*(long*)(ll.first->data) == data_elements[0]);
    }

    //dispose of the linked list
    dispose(linked_list_disposable_handle(&ll));

    //dispose of options
    dispose(linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}
