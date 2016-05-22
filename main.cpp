#include <iostream>
#include <cassert>
#include "fixedcache.hpp"
#include "lrucache.hpp"
#include "simple_cache.hpp"

using namespace std;
using namespace cache;

int main() {
    int n = 10;
    SimpleCache<int, int> fixed_cache(n);

    for (int i = 0; i < n * 10; ++i) {
        fixed_cache.Put(i, i);
        fixed_cache.Get(i);

        assert(n == fixed_cache.max_size());
    }

    return 0;
}
