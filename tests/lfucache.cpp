#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <set>
#include <thread>
#include <iostream>
#include <chrono>

#include "../lfucache.hpp"
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
        LFUCache<size_t, int> lru_cache(1);
        lru_cache.Put(1, 10);
        lru_cache.Put(2, 10);
        lru_cache.Put(3, 10);

        EXPECT_TRUE(lru_cache.max_size() == 1);
        EXPECT_TRUE(lru_cache.Get(3) == 10);

        try {
            lru_cache.Get(1);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}

        try {
            lru_cache.Get(2);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}
    }

    {
        LFUCache<size_t, int> lru_cache(2);
        lru_cache.Put(1, 10);
        lru_cache.Put(2, 20);
        EXPECT_TRUE(lru_cache.Get(1) == 10);
        lru_cache.Put(3, 30);
        EXPECT_TRUE(lru_cache.max_size() == 2);
        EXPECT_TRUE(lru_cache.Get(1) == 10);
        EXPECT_TRUE(lru_cache.Get(3) == 30);

        try {
            lru_cache.Get(2);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}
    }
}

TEST(LFUCache, RandomTests) {
    const int cntTests = 1000;
    for (int test = 0; test < cntTests; ++test) {
        const int n = rand() % 100 + 1;
        const int cntValues = (rand() % 10 + 1) * n;
//        cerr << "#" << test << " ; n = " << n << "; cntValues = " << cntValues << endl;

        LFUCache<size_t, int> lru_cache(n);
        vector<int> values(cntValues);
        generate(values.begin(), values.end(), rand);

        for (int i = 0; i < cntValues; ++i)
            lru_cache.Put(i, values[i]);

        // expect that last `n` values were saved
        EXPECT_EQ(n, lru_cache.max_size());
        for (int i = cntValues - 1; i > cntValues - n + 1; --i) {
            EXPECT_EQ(values[i], lru_cache.Get(i));
        }
    }
}
