#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <set>
#include <thread>
#include <iostream>
#include <chrono>

#include "../main/lfucache.hpp"
#include "base_tests.hpp"

using namespace std;
using namespace cache;

TEST(LFUCache, max_size) {
    MaxSizeTest<LFUCache>();
}

TEST(LFUCache, SimpleTest) {
    SimpleTest<LFUCache>();
}

TEST(LFUCache, BasicFunctionality) {
    {
        LFUCache<size_t, int> lfu_cache(1);
        lfu_cache.Put(1, 10);
        lfu_cache.Put(2, 10);
        lfu_cache.Put(3, 10);

        EXPECT_TRUE(lfu_cache.max_size() == 1);
        EXPECT_TRUE(lfu_cache.Get(3) == 10);

        try {
            lfu_cache.Get(1);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}

        try {
            lfu_cache.Get(2);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}
    }

    {
        LFUCache<size_t, int> lfu_cache(2);
        lfu_cache.Put(1, 10);
        lfu_cache.Put(2, 20);
        EXPECT_TRUE(lfu_cache.Get(1) == 10);
        lfu_cache.Put(3, 30);
        EXPECT_TRUE(lfu_cache.max_size() == 2);
        EXPECT_TRUE(lfu_cache.Get(1) == 10);
        EXPECT_TRUE(lfu_cache.Get(3) == 30);

        try {
            lfu_cache.Get(2);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}
    }
}

TEST(LFUCache, RandomTests) {
    const int cntTests = 1000;
    for (int test = 0; test < cntTests; ++test) {
        const int n = rand() % 100 + 1;
        const int cntValues = (rand() % 10 + 1) * n;

        LFUCache<size_t, int> lfu_cache(n);
        vector<int> values(cntValues);
        generate(values.begin(), values.end(), rand);

        for (int i = cntValues - 1; i >= 0; --i) {
            lfu_cache.Put(i, values[i]);
            if (i >= cntValues - n)
                EXPECT_EQ(lfu_cache.Get(i), values[i]);
        }

        // expect that last (`n` - 1) values and the first one were saved
        EXPECT_EQ(n, lfu_cache.max_size());
        for (int i = cntValues - 1; i > cntValues - n; --i)
            EXPECT_EQ(values[i], lfu_cache.Get(i));
        EXPECT_EQ(values[0], lfu_cache.Get(0));
    }
}
