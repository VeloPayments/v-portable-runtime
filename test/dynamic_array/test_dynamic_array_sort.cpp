/**
 * \file test_dynamic_array_sort.cpp
 *
 * Unit tests for dynamic_array_sort.
 *
 * \copyright 2017-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <algorithm>
#include <iostream>
#include <minunit/minunit.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

using namespace std;

class dynamic_array_sort_test {
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
        dispose(dynamic_array_options_disposable_handle(&options));
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int dynamic_array_options_init_status;
    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

TEST_SUITE(dynamic_array_sort_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    dynamic_array_sort_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

/**
 * Verify that dynamic_array_options_init succeeds.
 */
BEGIN_TEST_F(options_init)
    TEST_ASSERT(
        VPR_STATUS_SUCCESS == fixture.dynamic_array_options_init_status);
END_TEST_F()

/**
 * Create a dynamic array, add elements to it, and sort it.
 */
BEGIN_TEST_F(basic_test)
    int unsorted_values[] = { 7, 3, 9, 4, 10, 2, 5, 1, 8, 6 };
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    dynamic_array_t array;
    TEST_ASSERT(
        VPR_STATUS_SUCCESS
            == dynamic_array_init(&fixture.options, &array, 10, 0, NULL));

    //we should have a reserved space of 10
    TEST_ASSERT((size_t)10 == array.reserved_elements);
    //there should be no element instances
    TEST_ASSERT((size_t)0 == array.elements);

    //append 10 values to the array
    for (int i = 0; i < 10; ++i)
    {
        TEST_EXPECT(0 == dynamic_array_append(&array, unsorted_values + i));
    }

    //the reserved size should still be 10
    TEST_EXPECT((size_t)10 == array.reserved_elements);
    //the instance size should be 10
    TEST_EXPECT((size_t)10 == array.elements);

    //sorting should return 0
    TEST_ASSERT(0 == dynamic_array_sort(&array));

    //the 10 values in the array should be sorted
    int* intArr = (int*)array.array;
    for (int i = 0; i < 10; ++i)
    {
        TEST_EXPECT(sorted_values[i] == intArr[i]);
    }

    //dispose the array
    dispose(dynamic_array_disposable_handle(&array));
END_TEST_F()

/**
 * Test all permutations of sorting a 1 element array.
 */
BEGIN_TEST_F(sort1)
    int sorted_values[] = { 1 };
    int permutations[] = { 1 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 1, 0, NULL));

        //we should have a reserved space of 1
        TEST_ASSERT((size_t)1 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 1 values to the array
        for (int i = 0; i < 1; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 1
        TEST_EXPECT((size_t)1 == array.reserved_elements);
        //the instance size should be 1
        TEST_EXPECT((size_t)1 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 1 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 1; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 1));
END_TEST_F()

/**
 * Test all permutations of sorting a 2 element array.
 */
BEGIN_TEST_F(sort2)
    int sorted_values[] = { 1, 2 };
    int permutations[] = { 1, 2 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 2, 0, NULL));

        //we should have a reserved space of 2
        TEST_ASSERT((size_t)2 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 2 values to the array
        for (int i = 0; i < 2; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 2
        TEST_EXPECT((size_t)2 == array.reserved_elements);
        //the instance size should be 2
        TEST_EXPECT((size_t)2 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 2 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 2; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 2));
END_TEST_F()

/**
 * Test all permutations of sorting a 3 element array.
 */
BEGIN_TEST_F(sort3)
    int sorted_values[] = { 1, 2, 3 };
    int permutations[] = { 1, 2, 3 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 3, 0, NULL));

        //we should have a reserved space of 3
        TEST_ASSERT((size_t)3 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 3 values to the array
        for (int i = 0; i < 3; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 3
        TEST_EXPECT((size_t)3 == array.reserved_elements);
        //the instance size should be 3
        TEST_EXPECT((size_t)3 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 3 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 3; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 3));
END_TEST_F()

/**
 * Test all permutations of sorting a 4 element array.
 */
BEGIN_TEST_F(sort4)
    int sorted_values[] = { 1, 2, 3, 4 };
    int permutations[] = { 1, 2, 3, 4 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 4, 0, NULL));

        //we should have a reserved space of 4
        TEST_ASSERT((size_t)4 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 4 values to the array
        for (int i = 0; i < 4; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 4
        TEST_EXPECT((size_t)4 == array.reserved_elements);
        //the instance size should be 4
        TEST_EXPECT((size_t)4 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 4 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 4; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 4));
END_TEST_F()

/**
 * Test all permutations of sorting a 5 element array.
 */
BEGIN_TEST_F(sort5)
    int sorted_values[] = { 1, 2, 3, 4, 5 };
    int permutations[] = { 1, 2, 3, 4, 5 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 5, 0, NULL));

        //we should have a reserved space of 5
        TEST_ASSERT((size_t)5 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 5 values to the array
        for (int i = 0; i < 5; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 5
        TEST_EXPECT((size_t)5 == array.reserved_elements);
        //the instance size should be 5
        TEST_EXPECT((size_t)5 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 5 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 5; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 5));
END_TEST_F()

/**
 * Test all permutations of sorting a 6 element array.
 */
BEGIN_TEST_F(sort6)
    int sorted_values[] = { 1, 2, 3, 4, 5, 6 };
    int permutations[] = { 1, 2, 3, 4, 5, 6 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 6, 0, NULL));

        //we should have a reserved space of 6
        TEST_ASSERT((size_t)6 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 6 values to the array
        for (int i = 0; i < 6; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 6
        TEST_EXPECT((size_t)6 == array.reserved_elements);
        //the instance size should be 6
        TEST_EXPECT((size_t)6 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 6 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 6; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 6));
END_TEST_F()

/**
 * Test all permutations of sorting a 7 element array.
 */
BEGIN_TEST_F(sort7)
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 7, 0, NULL));

        //we should have a reserved space of 7
        TEST_ASSERT((size_t)7 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 7 values to the array
        for (int i = 0; i < 7; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 7
        TEST_EXPECT((size_t)7 == array.reserved_elements);
        //the instance size should be 7
        TEST_EXPECT((size_t)7 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 7 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 7; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 7));
END_TEST_F()

/**
 * Test all permutations of sorting a 8 element array.
 */
BEGIN_TEST_F(sort8)
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 8, 0, NULL));

        //we should have a reserved space of 8
        TEST_ASSERT((size_t)8 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 8 values to the array
        for (int i = 0; i < 8; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 8
        TEST_EXPECT((size_t)8 == array.reserved_elements);
        //the instance size should be 8
        TEST_EXPECT((size_t)8 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 8 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 8; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 8));
END_TEST_F()

/**
 * Test all permutations of sorting a 9 element array.
 */
BEGIN_TEST_F(sort9)
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 9, 0, NULL));

        //we should have a reserved space of 9
        TEST_ASSERT((size_t)9 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 9 values to the array
        for (int i = 0; i < 9; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 9
        TEST_EXPECT((size_t)9 == array.reserved_elements);
        //the instance size should be 9
        TEST_EXPECT((size_t)9 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 9 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 9; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 9));
END_TEST_F()

/**
 * Test all permutations of sorting a 10 element array.
 */
BEGIN_TEST_F(sort10)
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    do
    {
        dynamic_array_t array;
        TEST_ASSERT(
            VPR_STATUS_SUCCESS
                == dynamic_array_init(&fixture.options, &array, 10, 0, NULL));

        //we should have a reserved space of 10
        TEST_ASSERT((size_t)10 == array.reserved_elements);
        //there should be no element instances
        TEST_ASSERT((size_t)0 == array.elements);

        //append 10 values to the array
        for (int i = 0; i < 10; ++i)
        {
            TEST_ASSERT(0 == dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 10
        TEST_EXPECT((size_t)10 == array.reserved_elements);
        //the instance size should be 10
        TEST_EXPECT((size_t)10 == array.elements);

        //sorting should return 0
        TEST_ASSERT(0 == dynamic_array_sort(&array));

        //the 10 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 10; ++i)
        {
            TEST_ASSERT(sorted_values[i] == intArr[i]);
        }

        //dispose the array
        dispose(dynamic_array_disposable_handle(&array));

    } while (next_permutation(permutations, permutations + 10));
END_TEST_F()
