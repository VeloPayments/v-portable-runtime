/**
 * \file test_dynamic_array_init.cpp
 *
 * Unit tests for dynamic_array_init.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

class dynamic_array_init_test {
public:
    void setUp()
    {
        malloc_allocator_options_init(&alloc_opts);
        dynamic_array_options_init_status =
            dynamic_array_options_init(
                &options, &alloc_opts, sizeof(int), &compare_int);
    }

    void tearDown()
    {
        if (VPR_STATUS_SUCCESS == dynamic_array_options_init_status)
        {
            dispose(dynamic_array_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int dynamic_array_options_init_status;
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

TEST_SUITE(dynamic_array_init_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dynamic_array_init_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

BEGIN_TEST_F(options_init)
    TEST_ASSERT(
        VPR_STATUS_SUCCESS == fixture.dynamic_array_options_init_status);
END_TEST_F()

BEGIN_TEST_F(basic_test)
    int SEVENTEEN = 17;
    dynamic_array_t array;

    //create an array of ints that is 10 elements in size all initialized with
    //our copy value.
    TEST_EXPECT(
        0 == dynamic_array_init(&fixture.options, &array, 10, 10, &SEVENTEEN));

    //iterate over the array, checking that each value matches
    const int* intarr = (const int*)array.array;
    for (int i = 0; i < 10; ++i)
    {
        TEST_EXPECT(SEVENTEEN == intarr[i]);
    }

    //dispose of our array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()
