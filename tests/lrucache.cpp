#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <set>
#include <thread>
#include <iostream>
#include <chrono>

#include <fixedcache.hpp>
#include <lrucache.hpp>

using namespace std;
using namespace cache;

TEST(LRUCache, max_size) {
    const int n = 10;
    LRUCache<int, int> lru_cache(n);

    for (int i = 0; i < n * 10; ++i) {
        lru_cache.Put(i, i);
        lru_cache.Get(i);

        EXPECT_EQ(n, lru_cache.max_size());
    }
}

TEST(LRUCache, BaseFunctionality) {
    const int n = 10;
    LRUCache<size_t, int> lru_cache(n);

    vector<int> values(n);
    generate(values.begin(), values.end(), rand);

    for (size_t i = 0; i < n; ++i) {
        lru_cache.Put(i, values[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        EXPECT_EQ(values[i], lru_cache.Get(i));
    }
}
