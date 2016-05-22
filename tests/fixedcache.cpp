#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <set>
#include <thread>
#include <iostream>
#include <chrono>

#include "../simple_cache.hpp"

using namespace std;
using namespace cache;

TEST(SimpleCache, max_size) {
    const int n = 10;
    SimpleCache<int, int> fixed_cache(n);

    for (int i = 0; i < n * 10; ++i) {
        fixed_cache.Put(i, i);
        fixed_cache.Get(i);

        EXPECT_EQ(n, fixed_cache.max_size());
    }
}

TEST(SimpleCache, BaseFunctionality) {
    const int n = 10;
    SimpleCache<size_t, int> fixed_cache(n);

    vector<int> values(n);
    generate(values.begin(), values.end(), rand);

    for (size_t i = 0; i < n; ++i) {
        fixed_cache.Put(i, values[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        EXPECT_EQ(values[i], fixed_cache.Get(i));
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
