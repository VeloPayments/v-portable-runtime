/**
 * \file test_bloom_filter_init.cpp
 *
 * Unit tests for bloom_filter_init.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <time.h>
#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/bloom_filter.h>

static void generate_random_string(char*, int);
static int hamming_distance(uint64_t, uint64_t);
static void test_hash_function(hash_func_t);
static void test_distribution(uint64_t*, int, double, double);

class bloom_filter_hash_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init(&options, &alloc_opts, 8);
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
};

// ensure our utility function to measure hamming distances is correct
TEST(hamming_distance_test, test_utility_function)
{
    ASSERT_EQ(hamming_distance(0, 0), 0);
    ASSERT_EQ(hamming_distance(0, 1), 1);
    ASSERT_EQ(hamming_distance(0, 3), 2);
    ASSERT_EQ(hamming_distance(7, 1), 2);

    uint64_t max32 = 0xFFFFFFFF;
    ASSERT_EQ(hamming_distance(0, max32), 32);

    uint64_t max64 = 0xFFFFFFFFFFFFFFFF;

    ASSERT_EQ(hamming_distance(0, max64), 64);
    ASSERT_EQ(hamming_distance(max64, 0), 64);

    ASSERT_EQ(hamming_distance(max32, max64), 32);
}

TEST_F(bloom_filter_hash_test, hash_func1_distribution_test)
{
    test_hash_function(options.hash_function_1);
}

TEST_F(bloom_filter_hash_test, hash_func2_distribution_test)
{
    test_hash_function(options.hash_function_2);
}

TEST_F(bloom_filter_hash_test, hash_function_interdependence)
{
    // the idea here is that the two hash functions should be
    // unrelated, and therefore produce hashed values for the
    // same input that are, on average, 32 bits apart

    const int n = 10000;
    srand(time(0));  // seed rng with current time
    long total_distance = 0L;

    for (int i = 0; i < n; i++)
    {
        // generate a random string
        int len = rand() % 100 + 2;
        char buffer[len];
        generate_random_string(buffer, len);

        // calculate the hash values using both functions
        uint64_t hash_val1 = options.hash_function_1(buffer);
        uint64_t hash_val2 = options.hash_function_2(buffer);

        total_distance += hamming_distance(hash_val1, hash_val2);
    }

    double mean_distance = (double)total_distance / n;
    EXPECT_GE(mean_distance, 31.0);
    EXPECT_LE(mean_distance, 33.0);
}

TEST_F(bloom_filter_hash_test, basic_test)
{
    int n = 10000;  // number of insertions
    uint64_t m = 0xFFFFFFFF;  // max 32 bits

    // generate a string to hash
    char buf[32];
    generate_random_string(buf, 32);

    // test that the hash value changes with n, and is < m
    uint64_t hashed_vals[n];

    for (int i = 0; i < n; i++)
    {
        hashed_vals[i] = bloom_filter_hash(&options, buf, i, m);
        ASSERT_LT(hashed_vals[i], m);

        for (int j = i + 1; j < i; j++)
        {
            ASSERT_NE(hashed_vals[i], hashed_vals[j]);
        }
    }

    // the hash values produced by each of the hash functions
    // should be distributed over the state space
    // meaning the mean hamming distance should be 16 bits (half of our m)
    test_distribution(hashed_vals, n, 15.0, 17.0);
}


static void generate_random_string(char* buf, int len)
{
    // generate random characters 1-255
    for (int i = 0; i < (len - 1); i++)
    {
        buf[i] = (char)(rand() % 254 + 1);
    }

    // null terminate
    buf[len - 1] = 0;
}

static int hamming_distance(uint64_t x, uint64_t y)
{
    int dist = 0;

    uint64_t val = x ^ y;

    while (val != (uint64_t)0)
    {
        ++dist;
        val &= val - 1;
    }

    return dist;
}

static void test_hash_function(hash_func_t hash_func)
{
    const char* data = "this is some data that needs to be hashed.";

    uint64_t hash_val = hash_func(data);

    // verify the hash is repeatable
    for (int i = 0; i < 10; i++)
    {
        ASSERT_EQ(hash_func(data), hash_val);
    }

    // calculate the average hamming distance.  You would expect, on average,
    // half of the bits to flip from one key to the next.  The idea here is
    // to get several random values, hash them, then for each hashed value
    // measure the hamming distance to other values and ensure it is very
    // close to the expected mean

    const int n = 10000;
    uint64_t hashed_vals[n];
    srand(time(0));  // seed rng with current time
    for (int i = 0; i < n; i++)
    {
        // generate a random length >= 2
        int len = rand() % 100 + 2;
        char buffer[len];
        generate_random_string(buffer, len);
        hashed_vals[i] = hash_func(buffer);
    }

    test_distribution(hashed_vals, n, 31.0, 33.0);
}

static void test_distribution(uint64_t* vals, int num_vals,
    double lower, double upper)
{
    long total_distance = 0L;
    int num_distances = 0;
    for (int i = 0; i < num_vals; i++)
    {
        for (int j = i + 1; j < num_vals; j++)
        {
            total_distance += hamming_distance(vals[i], vals[j]);
            ++num_distances;
        }
    }

    double mean_distance = (double)total_distance / num_distances;
    EXPECT_GE(mean_distance, lower);
    EXPECT_LE(mean_distance, upper);
}
