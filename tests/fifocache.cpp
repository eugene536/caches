#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <set>
#include <thread>
#include <iostream>
#include <chrono>

#include <fifocache.hpp>
#include "base_tests.hpp"

using namespace std;
using namespace cache;

TEST(FIFOCache, max_size) {
    MaxSizeTest<FIFOCache>();
}

TEST(FIFOCache, SimpleTest) {
    SimpleTest<FIFOCache>();
}

TEST(FIFOCache, BasicFunctionality) {
    {
        FIFOCache<size_t, int> fifo_cache(1);
        fifo_cache.Put(1, 10);
        fifo_cache.Put(2, 10);
        fifo_cache.Put(3, 10);

        EXPECT_TRUE(fifo_cache.max_size() == 1);
        EXPECT_TRUE(fifo_cache.Get(3) == 10);

        try {
            fifo_cache.Get(1);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}

        try {
            fifo_cache.Get(2);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}
    }

    {
        FIFOCache<size_t, int> fifo_cache(2);
        fifo_cache.Put(1, 10);
        fifo_cache.Put(2, 20);
        EXPECT_TRUE(fifo_cache.Get(1) == 10);
        fifo_cache.Put(3, 30);
        EXPECT_TRUE(fifo_cache.max_size() == 2);
        EXPECT_TRUE(fifo_cache.Get(2) == 20);
        EXPECT_TRUE(fifo_cache.Get(3) == 30);

        try {
            fifo_cache.Get(1);
            ASSERT_TRUE(false);
        } catch(std::out_of_range& e) {}
    }
}

TEST(FIFOCache, RandomTests) {
    const int cntTests = 1000;
    for (int test = 0; test < cntTests; ++test) {
        const int n = rand() % 100 + 1;
        const int cntValues = (rand() % 10 + 1) * n;
//        cerr << "#" << test << " ; n = " << n << "; cntValues = " << cntValues << endl;

        FIFOCache<size_t, int> fifo_cache(n);
        vector<int> values(cntValues);
        generate(values.begin(), values.end(), rand);

        for (int i = 0; i < cntValues; ++i)
            fifo_cache.Put(i, values[i]);

        // expect that last `n` values were saved
        EXPECT_EQ(n, fifo_cache.max_size());
        for (int i = cntValues - 1; i > cntValues - n + 1; --i) {
            EXPECT_EQ(values[i], fifo_cache.Get(i));
        }
    }
}

TEST(FIFOCache, ThreadSafety) {
    ThreadSafetyTest<FIFOCache>();
}
