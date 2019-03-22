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
static void generate_random_string(char*, int);

class bloom_filter_test : public ::testing::Test {
protected:
    void SetUp(size_t size_in_bytes, int num_hash_functions)
    {
        malloc_allocator_options_init(&alloc_opts);
        bloom_filter_options_init(&options, &alloc_opts, size_in_bytes,
            num_hash_functions);
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

TEST_F(bloom_filter_test, init_test)
{
    SetUp(4, 4);
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    EXPECT_EQ(bloom.options->size_in_bytes, (size_t)4);
    EXPECT_NE(bloom.bitmap, nullptr);

    // verify the bitmap is initialized to all 0s
    char testblock[bloom.options->size_in_bytes];
    memset(testblock, 0, bloom.options->size_in_bytes);
    EXPECT_EQ(
        memcmp(testblock, bloom.bitmap, bloom.options->size_in_bytes), 0);

    //dispose of our list
    dispose((disposable_t*)&bloom);
}

TEST_F(bloom_filter_test, simple_add_item_test)
{
    SetUp(4, 4);
    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    const char* data = "add me to the bloom filter!";

    // initially that is NOT in the filter
    EXPECT_FALSE(bloom_filter_contains_item(&bloom, data));


    ASSERT_EQ(bloom_filter_add_item(&bloom, data), 0);

    // now the item should be in the filter
    // (no false positives possible when its the only item!)
    EXPECT_TRUE(bloom_filter_contains_item(&bloom, data));

    //dispose of our list
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
    SetUp(7794, 5);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    verify_false_positive_error_rate(&bloom, 0.05, 10000);
}

TEST_F(bloom_filter_test, false_positive_error_rate_1pct_test)
{
    SetUp(11981, 7);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    verify_false_positive_error_rate(&bloom, 0.01, 10000);
}

TEST_F(bloom_filter_test, false_positive_error_rate_half_pct_test)
{
    SetUp(13785, 8);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    verify_false_positive_error_rate(&bloom, 0.005, 10000);
}

TEST_F(bloom_filter_test, false_positive_error_rate_volume_test)
{
    SetUp(1198132, 7);

    bloom_filter bloom;

    ASSERT_EQ(bloom_filter_init(&options, &bloom), 0);

    verify_false_positive_error_rate(&bloom, 0.01, 1000000);
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

static void verify_false_positive_error_rate(bloom_filter* bloom, double target,
    const int n)
{
    for (int i = 0; i < n; i++)
    {
        char buf[17];
        generate_random_string(buf, 17);
        bloom_filter_add_item(bloom, buf);
        ASSERT_TRUE(bloom_filter_contains_item(bloom, buf));
    }


    // generate n more random items and ask if they are
    // in the filter.  none really are.  if it says no, no problem.
    // measure the false positive rate
    int false_positives = 0;
    for (int i = 0; i < n; i++)
    {
        char buf[17];
        generate_random_string(buf, 17);
        if (bloom_filter_contains_item(bloom, buf))
        {
            ++false_positives;
        }
    }

    printf("false positives: target: %0.4f, actual% 0.4f, raw: %i out of %i\n",
        target, (double)false_positives / n, false_positives, n);
}
