#include <cassert>
#include <iostream>

#include <lfucache.hpp>
#include <lrucache.hpp>
#include <fifocache.hpp>

int main() {
    cache::LRUCache<int, int> lru_cache(1);
    lru_cache.Put(1, 10);
    std::cerr << "lru: " << lru_cache.Get(1) << std::endl;

    cache::LFUCache<int, int> lfu_cache(1);
    lfu_cache.Put(1, 10);
    std::cerr << "lfu: " << lfu_cache.Get(1) << std::endl;

    cache::FIFOCache<int, int> fifo_cache(1);
    fifo_cache.Put(1, 10);
    std::cerr << "fifo: " << fifo_cache.Get(1) << std::endl;

    return 0;
}
