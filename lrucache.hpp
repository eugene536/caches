#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP
#include <memory>
#include <fixedcache.hpp>

namespace cache {

template <class K, class V>
class LRUCacheEntry : public CacheEntry<V> {
public:
    LRUCacheEntry(const K& key, const V& value)
        : CacheEntry<V>(value)
        , key_(key)
    {}

private:
    K key_;
    LRUCacheEntry* next_;
    LRUCacheEntry* prev_;
};

template <class K, class V>
class LRUCache : public FixedCache<K, V> {
public:
    explicit LRUCache(size_t max_size)
        : FixedCache<K, V>(max_size)
    {}


    const V& GetImpl(const K& key) const override {
        return FixedCache<K, V>::GetImpl(key);
    }

    void PutImpl(const K& key, const V& value) override {
        FixedCache<K, V>::PutImpl(key, value);
    }

private:
    LRUCacheEntry<K, V>* head_;
    LRUCacheEntry<K, V>* tail_;
};

} //namespace cache

#endif // LRUCACHE_HPP
