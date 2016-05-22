#ifndef BASE_TESTS_HPP
#define BASE_TESTS_HPP

#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <vector>

template<template<class, class> class Cache>
void MaxSizeTest() {
    const int n = 10;
    Cache<int, int> cache(n);

    for (int i = 0; i < n * 10; ++i) {
        cache.Put(i, i);
        EXPECT_EQ(n, cache.max_size());

        cache.Get(i);
        EXPECT_EQ(n, cache.max_size());
    }
}

template<template<class, class> class Cache>
void SimpleTest() {
    const int n = 10;
    Cache<size_t, int> cache(n);

    std::vector<int> values(n);
    std::generate(values.begin(), values.end(), rand);

    for (size_t i = 0; i < n; ++i) {
        cache.Put(i, values[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        EXPECT_EQ(values[i], cache.Get(i));
    }
}

template<template<class, class> class Cache>
void ThreadSafetyTest() {
    using namespace std;

    const int kCntTests = 1000;
    for (int i = 0; i < kCntTests; ++i) {
        const int n = rand() % 100 + 1;
        Cache<int, int> cache(n);

        vector<int> temp(n);
        generate(temp.begin(), temp.end(), rand);

        const int kCntThreads = 4;
        vector<thread> threads(kCntThreads);

        for (thread& t: threads) {
            t = thread([&]() {
                for (int i = 0; i < n; ++i) {
                    cache.Put(i, temp[rand() % n]);
                }
            });
        }

        for (thread& t: threads)
            t.join();

        EXPECT_EQ(cache.max_size(), n);
        set<int> values(temp.begin(), temp.end());
        for (int i = 0; i < n; ++i)
            EXPECT_TRUE(values.count(cache.Get(i)) > 0);
    }
}

#endif // BASE_TESTS_HPP
