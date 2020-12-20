/**
 * \file doubly_linked_list_main.c
 *
 * Simple model check of doubly linked list
 *
 * \copyright 2019-2020 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/doubly_linked_list.h>

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
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize doubly linked list options
    doubly_linked_list_options_init(
        &options, &alloc_opts, false, sizeof(int), false);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(int)));

    //initialize the doubly linked list
    MODEL_ASSERT(0 == doubly_linked_list_init(&options, &dll));

    // verify the (empty) list is valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
    MODEL_ASSERT(0 == dll.elements);
    MODEL_ASSERT(NULL == dll.first);
    MODEL_ASSERT(NULL == dll.last);

    //dispose of the linked list
    dispose(doubly_linked_list_disposable_handle(&dll));

    //dispose of options
    dispose(doubly_linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

static void verify_singleton_list()
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    // initialize doubly linked list options
    // note: if we do copy_on_insert the last argument (release_on_dispose) is
    // ignored.  If we don't do copy_on_insert, we don't want to release_on_dispose
    // either as the data element is in stack memory.
    _Bool copy_on_insert = nondet_bool();
    doubly_linked_list_options_init(
        &options, &alloc_opts, copy_on_insert, sizeof(int), copy_on_insert);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(int)));

    //initialize the list
    MODEL_ASSERT(0 == doubly_linked_list_init(&options, &dll));

    // add an item to the list
    int data = 42;
    if (0 == doubly_linked_list_insert_beginning(&dll, &data))
    {
        // verify the singleton list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
        MODEL_ASSERT(1 == dll.elements);
        MODEL_ASSERT(NULL != dll.first);
        MODEL_ASSERT(NULL != dll.last);
        MODEL_ASSERT(NULL != dll.first->data);
        MODEL_ASSERT(*(int*)(dll.first->data) == data);
    }

    //dispose of the linked list
    dispose(doubly_linked_list_disposable_handle(&dll));

    //dispose of options
    dispose(doubly_linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

static void verify_beginning_before()
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    // initialize doubly linked list options
    // note: if we do copy_on_insert the last argument (release_on_dispose) is
    // ignored.  If we don't do copy_on_insert, we don't want to release_on_dispose
    // either as the data element is in stack memory.
    _Bool copy_on_insert = nondet_bool();
    doubly_linked_list_options_init(
        &options, &alloc_opts, copy_on_insert, sizeof(long), copy_on_insert);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(long)));

    //initialize the list
    MODEL_ASSERT(0 == doubly_linked_list_init(&options, &dll));

    long data_elements[] = { 0, 1 };

    if (0 == doubly_linked_list_insert_beginning(&dll, &data_elements[0]) && 0 == doubly_linked_list_insert_before(&dll, dll.last, &data_elements[1]))
    {
        // verify the list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
        MODEL_ASSERT(2 == dll.elements);
        MODEL_ASSERT(NULL != dll.first);
        MODEL_ASSERT(NULL != dll.last);

        // verify pointers
        MODEL_ASSERT(dll.first->next->prev == dll.first);
        MODEL_ASSERT(dll.last->prev->next == dll.last);

        // verify data
        MODEL_ASSERT(*(long*)(dll.first->data) == data_elements[1]);
        MODEL_ASSERT(*(long*)(dll.last->data) == data_elements[0]);
    }

    //dispose of the linked list
    dispose(doubly_linked_list_disposable_handle(&dll));

    //dispose of options
    dispose(doubly_linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}

static void verify_after_end_remove()
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    // initialize doubly linked list options
    // note: if we do copy_on_insert the last argument (release_on_dispose) is
    // ignored.  If we don't do copy_on_insert, we don't want to release_on_dispose
    // either as the data element is in stack memory.
    _Bool copy_on_insert = nondet_bool();
    doubly_linked_list_options_init(
        &options, &alloc_opts, copy_on_insert, sizeof(long), copy_on_insert);

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(long)));

    //initialize the list
    MODEL_ASSERT(0 == doubly_linked_list_init(&options, &dll));

    long data_elements[] = { 2, 3 };

    if (0 == doubly_linked_list_insert_end(&dll, &data_elements[0]) && 0 == doubly_linked_list_insert_after(&dll, dll.last, &data_elements[1]))
    {
        doubly_linked_list_element_t* elem = dll.last->prev;
        doubly_linked_list_remove(&dll, elem);
        // we now "own" the element we removed
        if (copy_on_insert)
        {
            release(&alloc_opts, elem->data);
        }
        release(&alloc_opts, elem);

        // verify the list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
        MODEL_ASSERT(1 == dll.elements);
        MODEL_ASSERT(NULL != dll.first);
        MODEL_ASSERT(NULL != dll.last);
        MODEL_ASSERT(dll.first == dll.last);

        // verify pointers
        MODEL_ASSERT(NULL == dll.first->next);
        MODEL_ASSERT(NULL == dll.first->prev);

        // verify data
        MODEL_ASSERT(*(long*)(dll.first->data) == data_elements[1]);
    }

    //dispose of the linked list
    dispose(doubly_linked_list_disposable_handle(&dll));

    //dispose of options
    dispose(doubly_linked_list_options_disposable_handle(&options));

    //dispose of allocator
    dispose(allocator_options_disposable_handle(&alloc_opts));
}
