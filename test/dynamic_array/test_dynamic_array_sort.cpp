/**
 * \file test_dynamic_array_sort.cpp
 *
 * Unit tests for dynamic_array_sort.
 *
 * \copyright 2017 Velo-Payments, Inc.  All rights reserved.
 */

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/compare.h>
#include <vpr/dynamic_array.h>

using namespace std;

class dynamic_array_sort_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        dynamic_array_options_init(
            &options, &alloc_opts, sizeof(int), &compare_int);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    dynamic_array_options_t options;
};

/**
 * Create a dynamic array, add elements to it, and sort it.
 */
TEST_F(dynamic_array_sort_test, basic_test)
{
    int unsorted_values[] = { 7, 3, 9, 4, 10, 2, 5, 1, 8, 6 };
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    dynamic_array_t array;
    dynamic_array_init(&options, &array, 10, 0, NULL);

    //we should have a reserved space of 10
    ASSERT_EQ((size_t)10, array.reserved_elements);
    //there should be no element instances
    ASSERT_EQ((size_t)0, array.elements);

    //append 10 values to the array
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(0, dynamic_array_append(&array, unsorted_values + i));
    }

    //the reserved size should still be 10
    EXPECT_EQ((size_t)10, array.reserved_elements);
    //the instance size should be 10
    EXPECT_EQ((size_t)10, array.elements);

    //sorting should return 0
    ASSERT_EQ(0, dynamic_array_sort(&array));

    //the 10 values in the array should be sorted
    int* intArr = (int*)array.array;
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(sorted_values[i], intArr[i]);
    }

    //dispose the array
    dispose((disposable_t*)&array);
}

/**
 * Test all permutations of sorting a 1 element array.
 */
TEST_F(dynamic_array_sort_test, sort1)
{
    int sorted_values[] = { 1 };
    int permutations[] = { 1 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 1, 0, NULL);

        //we should have a reserved space of 1
        ASSERT_EQ((size_t)1, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 1 values to the array
        for (int i = 0; i < 1; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 1
        EXPECT_EQ((size_t)1, array.reserved_elements);
        //the instance size should be 1
        EXPECT_EQ((size_t)1, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 1 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 1; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 1));
}

/**
 * Test all permutations of sorting a 2 element array.
 */
TEST_F(dynamic_array_sort_test, sort2)
{
    int sorted_values[] = { 1, 2 };
    int permutations[] = { 1, 2 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 2, 0, NULL);

        //we should have a reserved space of 2
        ASSERT_EQ((size_t)2, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 2 values to the array
        for (int i = 0; i < 2; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 2
        EXPECT_EQ((size_t)2, array.reserved_elements);
        //the instance size should be 2
        EXPECT_EQ((size_t)2, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 2 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 2; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 2));
}

/**
 * Test all permutations of sorting a 3 element array.
 */
TEST_F(dynamic_array_sort_test, sort3)
{
    int sorted_values[] = { 1, 2, 3 };
    int permutations[] = { 1, 2, 3 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 3, 0, NULL);

        //we should have a reserved space of 3
        ASSERT_EQ((size_t)3, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 3 values to the array
        for (int i = 0; i < 3; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 3
        EXPECT_EQ((size_t)3, array.reserved_elements);
        //the instance size should be 3
        EXPECT_EQ((size_t)3, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 3 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 3; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 3));
}

/**
 * Test all permutations of sorting a 4 element array.
 */
TEST_F(dynamic_array_sort_test, sort4)
{
    int sorted_values[] = { 1, 2, 3, 4 };
    int permutations[] = { 1, 2, 3, 4 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 4, 0, NULL);

        //we should have a reserved space of 4
        ASSERT_EQ((size_t)4, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 4 values to the array
        for (int i = 0; i < 4; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 4
        EXPECT_EQ((size_t)4, array.reserved_elements);
        //the instance size should be 4
        EXPECT_EQ((size_t)4, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 4 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 4; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 4));
}

/**
 * Test all permutations of sorting a 5 element array.
 */
TEST_F(dynamic_array_sort_test, sort5)
{
    int sorted_values[] = { 1, 2, 3, 4, 5 };
    int permutations[] = { 1, 2, 3, 4, 5 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 5, 0, NULL);

        //we should have a reserved space of 5
        ASSERT_EQ((size_t)5, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 5 values to the array
        for (int i = 0; i < 5; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 5
        EXPECT_EQ((size_t)5, array.reserved_elements);
        //the instance size should be 5
        EXPECT_EQ((size_t)5, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 5 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 5; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 5));
}

/**
 * Test all permutations of sorting a 6 element array.
 */
TEST_F(dynamic_array_sort_test, sort6)
{
    int sorted_values[] = { 1, 2, 3, 4, 5, 6 };
    int permutations[] = { 1, 2, 3, 4, 5, 6 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 6, 0, NULL);

        //we should have a reserved space of 6
        ASSERT_EQ((size_t)6, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 6 values to the array
        for (int i = 0; i < 6; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 6
        EXPECT_EQ((size_t)6, array.reserved_elements);
        //the instance size should be 6
        EXPECT_EQ((size_t)6, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 6 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 6; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 6));
}

/**
 * Test all permutations of sorting a 7 element array.
 */
TEST_F(dynamic_array_sort_test, sort7)
{
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 7, 0, NULL);

        //we should have a reserved space of 7
        ASSERT_EQ((size_t)7, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 7 values to the array
        for (int i = 0; i < 7; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 7
        EXPECT_EQ((size_t)7, array.reserved_elements);
        //the instance size should be 7
        EXPECT_EQ((size_t)7, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 7 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 7; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 7));
}

/**
 * Test all permutations of sorting a 8 element array.
 */
TEST_F(dynamic_array_sort_test, sort8)
{
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 8, 0, NULL);

        //we should have a reserved space of 8
        ASSERT_EQ((size_t)8, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 8 values to the array
        for (int i = 0; i < 8; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 8
        EXPECT_EQ((size_t)8, array.reserved_elements);
        //the instance size should be 8
        EXPECT_EQ((size_t)8, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 8 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 8; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 8));
}

/**
 * Test all permutations of sorting a 9 element array.
 */
TEST_F(dynamic_array_sort_test, sort9)
{
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 9, 0, NULL);

        //we should have a reserved space of 9
        ASSERT_EQ((size_t)9, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 9 values to the array
        for (int i = 0; i < 9; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 9
        EXPECT_EQ((size_t)9, array.reserved_elements);
        //the instance size should be 9
        EXPECT_EQ((size_t)9, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 9 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 9; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 9));
}

/**
 * Test all permutations of sorting a 10 element array.
 */
TEST_F(dynamic_array_sort_test, sort10)
{
    int sorted_values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int permutations[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    do
    {
        dynamic_array_t array;
        dynamic_array_init(&options, &array, 10, 0, NULL);

        //we should have a reserved space of 10
        ASSERT_EQ((size_t)10, array.reserved_elements);
        //there should be no element instances
        ASSERT_EQ((size_t)0, array.elements);

        //append 10 values to the array
        for (int i = 0; i < 10; ++i)
        {
            ASSERT_EQ(0, dynamic_array_append(&array, permutations + i));
        }

        //the reserved size should still be 10
        EXPECT_EQ((size_t)10, array.reserved_elements);
        //the instance size should be 10
        EXPECT_EQ((size_t)10, array.elements);

        //sorting should return 0
        ASSERT_EQ(0, dynamic_array_sort(&array));

        //the 10 values in the array should be sorted
        int* intArr = (int*)array.array;
        for (int i = 0; i < 10; ++i)
        {
            ASSERT_EQ(sorted_values[i], intArr[i]);
        }

        //dispose the array
        dispose((disposable_t*)&array);

    } while (next_permutation(permutations, permutations + 10));
}
