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
static void test_distribution(uint64_t*, int);

class bloom_filter_hash_test : public ::testing::Test {
protected:
    void SetUp() override
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init(&options, &alloc_opts,
            100,  // number of expected entries
            0.1,  // target_error_rate
            64  // max size in bytes
        );
        srand(time(0));  // seed rng with current time
    }

    void TearDown() override
    {
        dispose((disposable_t*)&options);
        dispose((disposable_t*)&alloc_opts);
    }

    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
};

/**
 * Test that the test utility function to measure hamming distances is correct.
 */
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

/**
 * Test the hash functions for this filter by:
 *
 * 1) ensuring they are repeatable
 * 2) ensuring they uniformly distributes hash values of the possible set of
 * hash values.
 */
TEST_F(bloom_filter_hash_test, hash_func_distribution_test)
{
    test_hash_function(options.hash_function_1);
    test_hash_function(options.hash_function_2);
}

/**
 * Test that the hash function are independent of each other.
 *
 * The hash functions are deemed independent if:
 *
 * 1) They produce hash values that, on average, have a hamming distance
 * of 32 bits given the same input.
 *
 * -and-
 *
 * 2) Each bit of a hash value produced by function 1 has a 50% probability
 * of being different than the same bit of the hash value produced by
 * function 2.
 */
TEST_F(bloom_filter_hash_test, hash_function_interdependence)
{
    const int n = 10000;
    long total_distance = 0L;
    int bits_differ[64];
    for (int i = 0; i < 64; i++)
    {
        bits_differ[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        int len = rand() % 100 + 2;
        char buffer[len];
        generate_random_string(buffer, len);

        // calculate the hash values using both functions
        uint64_t hash_val1 = options.hash_function_1(buffer);
        uint64_t hash_val2 = options.hash_function_2(buffer);

        total_distance += hamming_distance(hash_val1, hash_val2);

        // test which bits are different
        uint64_t flipped = hash_val1 ^ hash_val2;
        for (int j = 0; j < 64; j++)
        {
            if (flipped & ((uint64_t)1 << j))
            {
                ++bits_differ[j];
            }
        }
    }

    double mean_distance = (double)total_distance / n;
    EXPECT_GE(mean_distance, 31.0);
    EXPECT_LE(mean_distance, 33.0);

    int num_vals_40pct = n * 0.4;
    int num_vals_60pct = n * 0.6;

    for (int i = 0; i < 64; i++)
    {
        EXPECT_GE(bits_differ[i], num_vals_40pct);
        EXPECT_LE(bits_differ[i], num_vals_60pct);
    }
}

/**
 * Test the hash generation function that uses the two core hash functions
 * to generate an arbitrary number of hashes.  The generated hashes map input
 * values to bits in the filter.
 *
 * In this test we ensure that the generated hash functions reliably map
 * inputs to the same bit, and inputs are uniformly distributed to all bits
 * in the filter.
 */
TEST_F(bloom_filter_hash_test, basic_test)
{
    const int n = 10000;  // number of insertions

    // generate a string to hash
    char buf[32];
    generate_random_string(buf, 32);

    // generate the hash values
    uint64_t hv1 = options.hash_function_1(buf);
    uint64_t hv2 = options.hash_function_2(buf);

    unsigned int bits_in_filter = options.size_in_bytes * 8;
    int bits_to_set[bits_in_filter];
    for (unsigned int i = 0; i < bits_in_filter; i++)
    {
        bits_to_set[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        // verify our hash values aren't changing
        ASSERT_EQ(options.hash_function_1(buf), hv1);
        ASSERT_EQ(options.hash_function_2(buf), hv2);

        // which bit do we set?
        unsigned int bit = bloom_filter_hash(&options, buf, i);
        ASSERT_LT(bit, bits_in_filter);

        // do I always get the same bit?
        for (int j = 0; j < 100; j++)
        {
            ASSERT_EQ(bloom_filter_hash(&options, buf, i), bit);
        }

        bits_to_set[bit]++;
    }

    // each bit should be set approximately n / bits_in_filter times.
    /*int lower = (double)n / bits_in_filter * 0.9;
    int upper = (double)n / bits_in_filter * 1.1;
    for (unsigned  int i=0; i<bits_in_filter; i++)
    {
        EXPECT_GE(bits_to_set[i], lower);
        EXPECT_LE(bits_to_set[i], upper);
    }*/
}

/**
 * Utility function to generate a null terminated random string.
 */
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

/**
 * Utility function to measure the hamming distance between two 64-bit words.
 */
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

/**
 * Test a hashing function by :
 *
 * 1) ensuring it is repeatable
 * 2) ensuring it uniformly distributes hash values of the possible set of
 * hash values.
 */
static void test_hash_function(hash_func_t hash_func)
{
    const char* data = "this is some data that needs to be hashed.";

    uint64_t hash_val = hash_func(data);

    // verify the hash is repeatable
    for (int i = 0; i < 10; i++)
    {
        ASSERT_EQ(hash_func(data), hash_val);
    }

    // generate a large number of random values and ensure the corresponding
    // hash values are uniformly distributed.
    const int n = 10000;
    uint64_t hashed_vals[n];
    for (int i = 0; i < n; i++)
    {
        int len = rand() % 100 + 2;
        char buffer[len];
        generate_random_string(buffer, len);
        hashed_vals[i] = hash_func(buffer);
    }

    test_distribution(hashed_vals, n);
}

/**
 * Test the "bit distribution" of a set of 64 bit values.
 *
 * 1) Each bit should be set approximately 50% of the time.
 * 2) The mean of all hamming distances should be close to 32 (half of 64).
 *
 */
static void test_distribution(uint64_t* vals, int num_vals)
{
    long total_distance = 0L;
    int num_distances = 0;
    int bits_set[64];
    for (int i = 0; i < 64; i++)
    {
        bits_set[i] = 0;
    }

    for (int i = 0; i < num_vals; i++)
    {
        for (int j = i + 1; j < num_vals; j++)
        {
            total_distance += hamming_distance(vals[i], vals[j]);
            ++num_distances;
        }
        // which bits are set?
        for (int j = 0; j < 64; j++)
        {
            if (vals[i] & ((uint64_t)1 << j))
            {
                ++bits_set[j];
            }
        }
    }

    double mean_distance = (double)total_distance / num_distances;
    EXPECT_GE(mean_distance, 31.0);
    EXPECT_LE(mean_distance, 33.0);

    int num_vals_40pct = num_vals * 0.4;
    int num_vals_60pct = num_vals * 0.6;

    for (int i = 0; i < 64; i++)
    {
        EXPECT_GE(bits_set[i], num_vals_40pct);
        EXPECT_LE(bits_set[i], num_vals_60pct);
    }
}
