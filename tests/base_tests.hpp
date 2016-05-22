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

#endif // BASE_TESTS_HPP
