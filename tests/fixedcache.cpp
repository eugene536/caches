#include "../main/simple_cache.hpp"
#include "base_tests.hpp"

using namespace std;
using namespace cache;

TEST(SimpleCache, max_size) {
    MaxSizeTest<SimpleCache>();
}

TEST(SimpleCache, SimpleTest) {
    SimpleTest<SimpleCache>();
}
