#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP
#include <memory>
#include <list>
#include <exception>
#include <sstream>
#include <unordered_map>
#include "fixedcache.hpp"

namespace cache {

template <class K, class V>
class LRUCache : public FixedCache<K, V> {
private:
    typedef std::pair<K, V> Entry;
    typedef typename std::list<Entry>::iterator EntryIter;
    typedef typename std::unordered_map<K, EntryIter>::iterator CacheIter;
    typedef typename std::unordered_map<K, EntryIter>::const_iterator CacheConstIter;
    typedef FixedCache<K, V> BaseCache;

public:
    explicit LRUCache(size_t max_size)
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

        EntryIter entryIter = cacheIter->second;
        entries_.splice(entries_.begin(), entries_, entryIter);

        return entryIter->second;
    }

    void PutImpl(const K& key, const V& value) override {
        CacheIter cacheIter = cache_.find(key);

        if (cacheIter == cache_.end()) {
            if (entries_.size() < this->max_size()) {
                entries_.push_front({key, value});
            } else {
                const K& lruKey = entries_.back().first;

                assert(cache_.find(lruKey) != cache_.end());
                cache_.erase(cache_.find(lruKey));

                entries_.splice(entries_.begin(), entries_, std::prev(entries_.end()));

                *entries_.begin() = {key, value};
            }

            cache_[key] = entries_.begin();
        } else { // if I found entry that I can just modify its value
            EntryIter entryIter = cacheIter->second;
            entryIter->second = value;
            entries_.splice(entries_.begin(), entries_, entryIter);
        }
    }

private:
    std::unordered_map<K, EntryIter> cache_;
    mutable std::list<Entry> entries_;
};

} //namespace cache

#endif // LRUCACHE_HPP
