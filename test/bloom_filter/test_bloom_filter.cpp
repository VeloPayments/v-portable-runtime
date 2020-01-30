/**
 * \file test_bloom_filter.cpp
 *
 * Unit tests for bloom_filter.
 *
 * \copyright 2019 Velo-Payments, Inc.  All rights reserved.
 */

#include <gtest/gtest.h>
#include <vpr/allocator/malloc_allocator.h>
#include <vpr/bloom_filter.h>

static void verify_false_positive_error_rate(bloom_filter*, double, const int);
static void generate_random_bytes(uint8_t*, size_t);

class bloom_filter_test : public ::testing::Test {
protected:
    void SetUp(unsigned int num_expected_entries, float target_error_rate,
        size_t max_size_in_bytes)
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init_status =
            bloom_filter_options_init(
                &options, &alloc_opts, num_expected_entries, target_error_rate,
                max_size_in_bytes);
        srand(time(0));  // seed rng with current time
    }

    void TearDown() override
    {
        if (VPR_STATUS_SUCCESS == bloom_filter_options_init_status)
        {
            dispose((disposable_t*)&options);
        }
        dispose((disposable_t*)&alloc_opts);
    }

    int bloom_filter_options_init_status;
    allocator_options_t alloc_opts;
    bloom_filter_options_t options;
};

/**
 * Test the bloom filter is initialized properly.
 */
TEST_F(bloom_filter_test, init_test)
{
    // verify that options were set up
    ASSERT_EQ(VPR_STATUS_SUCCESS, bloom_filter_options_init_status);

    // set up a bloom filter with 1000 expected entries, a 10% error rate,
    // and maximum size of 1024 bytes.
    SetUp(1000, 0.1, 1024);
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    ASSERT_EQ(bloom.options->num_expected_entries, 1000u);

    // the actual size of the bloom filter should be 600 bytes
    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)600);
    EXPECT_NE(bloom.bitmap, nullptr);

    // we should use three hash functions
    EXPECT_EQ(bloom.options->num_hash_functions, 3u);

    // verify the bitmap is initialized to all 0s
    char testblock[bloom.options->size_in_bytes];
    memset(testblock, 0, bloom.options->size_in_bytes);
    EXPECT_EQ(
        memcmp(testblock, bloom.bitmap, bloom.options->size_in_bytes), 0);

    //dispose of our filter
    dispose((disposable_t*)&bloom);
}

/**
 * Test the bloom filter initialization when there is not enough space
 * to reach the desired target error rate.
 *
 * A filter with 1000 items and an error rate of 0.1 requires 600 bytes.
 * In this case we will only allow 300 bytes.
 */
TEST_F(bloom_filter_test, not_enough_space)
{
    SetUp(1000, 0.1, 300);
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    ASSERT_EQ(bloom.options->num_expected_entries, 1000u);

    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)300);
    EXPECT_NE(bloom.bitmap, nullptr);

    // we should use two hash functions
    EXPECT_EQ(bloom.options->num_hash_functions, 2u);

    // verify the bitmap is initialized to all 0s
    char testblock[bloom.options->size_in_bytes];
    memset(testblock, 0, bloom.options->size_in_bytes);
    EXPECT_EQ(
        memcmp(testblock, bloom.bitmap, bloom.options->size_in_bytes), 0);

    //dispose of our filter
    dispose((disposable_t*)&bloom);
}


/**
 * Test that we can add an item and query for it.
 */
TEST_F(bloom_filter_test, simple_add_item_test)
{
    // set up a bloom filter with 1000 expected entries, a 10% error rate,
    // and maximum size of 1024 bytes.
    SetUp(1000, 0.1, 1024);
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    const char* data = "add me to the bloom filter!";
    size_t sz_data = strlen(data);

    // initially that is NOT in the filter
    EXPECT_FALSE(bloom_filter_contains_item(&bloom, data, sz_data));


    ASSERT_EQ(bloom_filter_add_item(&bloom, data, sz_data), 0);

    // now the item should be in the filter
    // (no false positives possible when its the only item!)
    EXPECT_TRUE(bloom_filter_contains_item(&bloom, data, sz_data));

    //dispose of our filter
    dispose((disposable_t*)&bloom);
}

/**
 * Test of the error rate.  Given a fault tolerance p of 0.05,
 * and a set cardinality n of 10k items, our filter size
 * should be:  m = -(n ln p) / (ln 2)^2  bits
 *               = 29,957 / 0.48 bits
 *               = 62,352  bits  (or 7794 bytes)
 *
 * and the number of hashes (k) should be:
 *             k = (m / n) ln 2
 *               = (62352 / 10000) ln 2
 *               = 4.3
 */
TEST_F(bloom_filter_test, false_positive_error_rate_5pct_test)
{
    SetUp(10000, 0.05, 8192);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)7795);
    EXPECT_EQ(bloom.options->num_hash_functions, 4u);


    verify_false_positive_error_rate(&bloom, 0.05, 10000);

    //dispose of our filter
    dispose((disposable_t*)&bloom);
}

/**
 * Test the error rate with a fault tolerance of 0.01.
 */
TEST_F(bloom_filter_test, false_positive_error_rate_1pct_test)
{
    SetUp(10000, 0.01, 16384);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)11982);
    EXPECT_EQ(bloom.options->num_hash_functions, 7u);

    verify_false_positive_error_rate(&bloom, 0.01, 10000);

    //dispose of our filter
    dispose((disposable_t*)&bloom);
}

/**
 * Test the error rate with a fault tolerance of 0.005.
 */
TEST_F(bloom_filter_test, false_positive_error_rate_half_pct_test)
{
    SetUp(10000, 0.005, 16384);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)13785);
    EXPECT_EQ(bloom.options->num_hash_functions, 8u);

    verify_false_positive_error_rate(&bloom, 0.005, 10000);

    //dispose of our filter
    dispose((disposable_t*)&bloom);
}

/**
 * Test the error rate with a fault tolerance of 0.01 and
 * one million insertions.
 */
TEST_F(bloom_filter_test, false_positive_error_rate_volume_test)
{
    SetUp(1000000, 0.01, 1198132);  // ~1.2 mb

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    verify_false_positive_error_rate(&bloom, 0.01, 1000000);

    //dispose of our filter
    dispose((disposable_t*)&bloom);
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
 * Utility function to measure the false positive error rate and compare it
 * to the expected error rate.
 */
static void verify_false_positive_error_rate(bloom_filter* bloom,
    double target, const int n)
{
    double lower = target * 0.7;
    double upper = target * 1.3;

    // if you put something in the filter, it should ALWAYS
    // say it is in the filter (no possibility of false negative)
    for (int i = 0; i < n; i++)
    {
        uint8_t buf[17];
        generate_random_bytes(buf, 17);
        ASSERT_EQ(VPR_STATUS_SUCCESS, bloom_filter_add_item(bloom, buf, 17));
        ASSERT_TRUE(bloom_filter_contains_item(bloom, buf, 17));
    }

    // since this test is probabilistic, we'll try it a few times
    // before concluding something is broken
    _Bool met_target = false;
    int attempts = 0;
    double false_positive_rate;

    do
    {
        // measure the false positive rate
        int false_positives = 0;
        for (int i = 0; i < n; i++)
        {
            uint8_t buf[17];
            generate_random_bytes(buf, 17);
            if (bloom_filter_contains_item(bloom, buf, 17))
            {
                ++false_positives;
            }
        }

        false_positive_rate = (double)false_positives / n;
        met_target = false_positive_rate >= lower &&
            false_positive_rate <= upper;

    } while (!met_target && ++attempts <= 5);

    EXPECT_GE(false_positive_rate, lower);
    EXPECT_LE(false_positive_rate, upper);
}
