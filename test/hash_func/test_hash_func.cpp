/**
 * \file test_hash_func.cpp
 *
 * Unit tests for hash functions
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

/* DISABLED GTEST */
#if 0
#include <vpr/hash_func.h>

static void generate_random_bytes(uint8_t*, size_t);
static int hamming_distance(uint64_t, uint64_t);
static void test_hash_function(hash_func_t);
static void test_distribution(uint64_t*, int);

/**
 * Test that the test utility function to measure hamming distances is correct.
 */
TEST(hamming_distance_testD, test_utility_functionD)
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
 * Test the sdbm hash function
 */
TEST(sdbm_test, test_hash_functions)
{
    test_hash_function(&sdbm);
}

/**
 * Test the jenkins hash function
 */
TEST(jenkins_test, test_hash_functions)
{
    test_hash_function(&jenkins);
}

/**
 * Utility function to generate a random sequence of bytes
 */
static void generate_random_bytes(uint8_t* buf, size_t len)
{
    for (unsigned int i = 0; i < len; i++)
    {
        buf[i] = rand();
    }
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
    uint8_t data[] = { 0x00, 0x11, 0x22 };
    size_t sz_data = sizeof(data);

    uint64_t hash_val = hash_func(data, sz_data);

    // verify the hash is repeatable
    for (int i = 0; i < 10; i++)
    {
        ASSERT_EQ(hash_func(data, sz_data), hash_val);
    }

    // generate a large number of random values and ensure the corresponding
    // hash values are uniformly distributed.
    const int n = 10000;
    uint64_t hashed_vals[n];
    for (int i = 0; i < n; i++)
    {
        int len = rand() % 100 + 2;
        uint8_t buffer[len];
        generate_random_bytes(buffer, len);
        hashed_vals[i] = hash_func(buffer, len);
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
#endif
