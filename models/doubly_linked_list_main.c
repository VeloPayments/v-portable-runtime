/**
 * \file custom_doubly_linked_list_init.c
 *
 * Simple model check of doubly_linked_list_init.
 *
 * \copyright 2019 Velo Payments, Inc.  All rights reserved.
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
        &options, &alloc_opts, sizeof(int));

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(int)));

    //initialize the array
    MODEL_ASSERT(doubly_linked_list_init(&options, &dll) == 0);

    // verify the (empty) list is valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
    MODEL_ASSERT(dll.elements == 0);
    MODEL_ASSERT(dll.first == NULL);
    MODEL_ASSERT(dll.last == NULL);

    //dispose of the linked list
    dispose((disposable_t*)&dll);

    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}

static void verify_singleton_list()
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize doubly linked list options
    doubly_linked_list_options_init(
        &options, &alloc_opts, sizeof(int));

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(int)));

    //initialize the list
    MODEL_ASSERT(doubly_linked_list_init(&options, &dll) == 0);

    // add an item to the list
    int data = 42;
    if (doubly_linked_list_insert_beginning(&dll, &data) == 0)
    {
        // verify the singleton list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
        MODEL_ASSERT(dll.elements == 1);
        MODEL_ASSERT(dll.first != NULL);
        MODEL_ASSERT(dll.last != NULL);
        MODEL_ASSERT(dll.first->data != NULL);
        MODEL_ASSERT(*(int*)(dll.first->data) == data);
    }

    //dispose of the linked list
    dispose((disposable_t*)&dll);

    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}

static void verify_beginning_before()
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize doubly linked list options
    doubly_linked_list_options_init(
        &options, &alloc_opts, sizeof(long));

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(long)));

    //initialize the list
    MODEL_ASSERT(doubly_linked_list_init(&options, &dll) == 0);

    long data_elements[] = { 0, 1 };

    if (doubly_linked_list_insert_beginning(&dll, &data_elements[0]) == 0 && doubly_linked_list_insert_before(&dll, dll.last, &data_elements[1]) == 0)
    {
        // verify the list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
        MODEL_ASSERT(dll.elements == 2);
        MODEL_ASSERT(dll.first != NULL);
        MODEL_ASSERT(dll.last != NULL);

        // verify pointers
        MODEL_ASSERT(dll.first->next->prev == dll.first);
        MODEL_ASSERT(dll.last->prev->next == dll.last);

        // verify data
        MODEL_ASSERT(*(long*)(dll.first->data) == data_elements[1]);
        MODEL_ASSERT(*(long*)(dll.last->data) == data_elements[0]);
    }

    //dispose of the linked list
    dispose((disposable_t*)&dll);

    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}

static void verify_after_end_remove()
{
    allocator_options_t alloc_opts;
    doubly_linked_list_options_t options;
    doubly_linked_list_t dll;

    //initialize the allocator
    malloc_allocator_options_init(&alloc_opts);

    //initialize doubly linked list options
    doubly_linked_list_options_init(
        &options, &alloc_opts, sizeof(long));

    // verify the options are valid
    MODEL_ASSERT(MODEL_PROP_VALID_DLL_OPTIONS(&options, sizeof(long)));

    //initialize the list
    MODEL_ASSERT(doubly_linked_list_init(&options, &dll) == 0);

    long data_elements[] = { 2, 3 };

    if (doubly_linked_list_insert_end(&dll, &data_elements[0]) == 0 && doubly_linked_list_insert_after(&dll, dll.last, &data_elements[1]) == 0)
    {
        doubly_linked_list_element_t* elem = dll.last->prev;
        doubly_linked_list_remove(&dll, elem);
        // we now "own" the element we removed
        release(&alloc_opts, elem->data);
        release(&alloc_opts, elem);

        // verify the list is valid
        MODEL_ASSERT(MODEL_PROP_VALID_DLL(&dll));
        MODEL_ASSERT(dll.elements == 1);
        MODEL_ASSERT(dll.first != NULL);
        MODEL_ASSERT(dll.last != NULL);
        MODEL_ASSERT(dll.first == dll.last);

        // verify pointers
        MODEL_ASSERT(dll.first->next == NULL);
        MODEL_ASSERT(dll.first->prev == NULL);

        // verify data
        MODEL_ASSERT(*(long*)(dll.first->data) == data_elements[1]);
    }

    //dispose of the linked list
    dispose((disposable_t*)&dll);

    //dispose of options
    dispose((disposable_t*)&options);

    //dispose of allocator
    dispose((disposable_t*)&alloc_opts);
}
