#ifndef FIFOCACHE_HPP
#define FIFOCACHE_HPP

#include <memory>
#include <queue>
#include <exception>
#include <sstream>
#include <unordered_map>
#include "fixedcache.hpp"

namespace cache {

template <class K, class V>
class FIFOCache : public FixedCache<K, V> {
private:
    typedef std::pair<K, V> Entry;
    typedef typename std::unordered_map<K, V>::iterator CacheIter;
    typedef typename std::unordered_map<K, V>::const_iterator CacheConstIter;
    typedef FixedCache<K, V> BaseCache;

public:
    explicit FIFOCache(size_t max_size)
        : BaseCache(max_size)
    {}

private:
    const V& GetImpl(const K& key) const override {
        CacheConstIter cacheIter = cache_.find(key);
        if (cacheIter == cache_.end()) {
            std::stringstream ss;
            ss << key;
            throw std::out_of_range("There is no such key: " + ss.str());
        }

        return cacheIter->second;
    }

    void PutImpl(const K& key, const V& value) override {
        CacheIter cacheIter = cache_.find(key);

        if (cacheIter == cache_.end()) {
            if (entries_.size() == this->max_size()) {
                const K& firstKey = entries_.front().first;

                assert(cache_.find(firstKey) != cache_.end());
                cache_.erase(cache_.find(firstKey));

                entries_.pop();
            }

            entries_.push({key, value});
            cache_[key] = value;
        } else {
            cacheIter->second = value;
        }
    }

private:
    std::unordered_map<K, V> cache_;
    mutable std::queue<Entry> entries_;
};

} //namespace cache

#endif // FIFOCACHE_HPP
