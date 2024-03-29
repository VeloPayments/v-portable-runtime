/**
 * \file test_bloom_filter_hash.cpp
 *
 * Unit tests for bloom_filter hashing.
 *
 * \copyright 2019-2023 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <string.h>
#include <time.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/bloom_filter.h>

static void generate_random_bytes(uint8_t*, size_t);
static int hamming_distance(uint64_t, uint64_t);

class bloom_filter_hash_test {
public:
    void setUp()
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init_status =
            bloom_filter_options_init(&options, &alloc_opts,
                100,  // number of expected entries
                0.1,  // target_error_rate
                64  // max size in bytes
            );
        srand(time(0));  // seed rng with current time
    }

    void tearDown()
    {
        if (VPR_STATUS_SUCCESS == bloom_filter_options_init_status)
        {
            dispose(bloom_filter_options_disposable_handle(&options));
        }
        dispose(allocator_options_disposable_handle(&alloc_opts));
    }

    int bloom_filter_options_init_status;
    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
};

TEST_SUITE(hamming_distance_test);

/**
 * Test that the test utility function to measure hamming distances is correct.
 */
TEST(test_utility_function)
{
    TEST_ASSERT(hamming_distance(0, 0) == 0);
    TEST_ASSERT(hamming_distance(0, 1) == 1);
    TEST_ASSERT(hamming_distance(0, 3) == 2);
    TEST_ASSERT(hamming_distance(7, 1) == 2);

    uint64_t max32 = 0xFFFFFFFF;
    TEST_ASSERT(hamming_distance(0, max32) == 32);

    uint64_t max64 = 0xFFFFFFFFFFFFFFFF;

    TEST_ASSERT(hamming_distance(0, max64) == 64);
    TEST_ASSERT(hamming_distance(max64, 0) == 64);

    TEST_ASSERT(hamming_distance(max32, max64) == 32);
}

TEST_SUITE(bloom_filter_hash_test);

#define BEGIN_TEST_F(name) \
TEST(name) \
{ \
    bloom_filter_hash_test fixture; \
    fixture.setUp();

#define END_TEST_F() \
    fixture.tearDown(); \
}

/**
 * Test that options init worked as expected.
 */
BEGIN_TEST_F(options_init)
    TEST_ASSERT(VPR_STATUS_SUCCESS == fixture.bloom_filter_options_init_status);
END_TEST_F()

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
BEGIN_TEST_F(hash_function_interdependence)
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
        uint8_t buffer[len];
        generate_random_bytes(buffer, len);

        // calculate the hash values using both functions
        uint64_t hash_val1 = fixture.options.hash_function_1(buffer, len);
        uint64_t hash_val2 = fixture.options.hash_function_2(buffer, len);

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
    TEST_EXPECT(mean_distance >= 31.0);
    TEST_EXPECT(mean_distance <= 33.0);

    int num_vals_40pct = n * 0.4;
    int num_vals_60pct = n * 0.6;

    for (int i = 0; i < 64; i++)
    {
        TEST_EXPECT(bits_differ[i] >= num_vals_40pct);
        TEST_EXPECT(bits_differ[i] <= num_vals_60pct);
    }
END_TEST_F()

/**
 * Test the hash generation function that uses the two core hash functions
 * to generate an arbitrary number of hashes.  The generated hashes map input
 * values to bits in the filter.
 *
 * In this test we ensure that the generated hash functions reliably map
 * inputs to the same bit, and inputs are uniformly distributed to all bits
 * in the filter.
 */
BEGIN_TEST_F(basic_test)
    const int n = 10000;  // number of insertions

    const char* data = "this is some test data to be added to the filter.";
    size_t sz_data = strlen(data);

    unsigned int bits_in_filter = fixture.options.size_in_bytes * 8;
    int bits_to_set[bits_in_filter];
    for (unsigned int i = 0; i < bits_in_filter; i++)
    {
        bits_to_set[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        // which bit do we set?
        unsigned int bit =
            bloom_filter_hash(&fixture.options, data, sz_data, i);
        TEST_ASSERT(bit < bits_in_filter);

        // do I always get the same bit?
        for (int j = 0; j < 100; j++)
        {
            TEST_ASSERT(
                bloom_filter_hash(&fixture.options, data, sz_data, i) == bit);
        }

        bits_to_set[bit]++;
    }

    // each bit should be set approximately n / bits_in_filter times.
    int lower = (double)n / bits_in_filter * 0.9;
    int upper = (double)n / bits_in_filter * 1.1;
    for (unsigned int i = 0; i < bits_in_filter; i++)
    {
        TEST_EXPECT(bits_to_set[i] >= lower);
        TEST_EXPECT(bits_to_set[i] <= upper);
    }
END_TEST_F()

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
