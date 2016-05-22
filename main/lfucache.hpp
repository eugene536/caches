#ifndef LFUCACHE_HPP
#define LFUCACHE_HPP

#include <memory>
#include <list>
#include <exception>
#include <sstream>
#include <unordered_map>
#include "fixedcache.hpp"

namespace cache {

template <class K, class V>
class LFUCache : public FixedCache<K, V> {
private:

    struct Entry {
        // usage frequency
        // The max frequency is capped at the cache size
        // to avoid creating more and more frequency list entries
        size_t fr;

        // position in the vector<EntryIter> given by frToIters_;
        size_t pos;

        K key;
        V value;
    };

    typedef typename std::list<Entry>::iterator EntryIter;
    typedef typename std::unordered_map<K, EntryIter>::iterator CacheIter;
    typedef typename std::unordered_map<K, EntryIter>::const_iterator CacheConstIter;
    typedef FixedCache<K, V> BaseCache;

public:
    explicit LFUCache(size_t max_size)
        : BaseCache(max_size)
        , minFr_(0)
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
        MoveToTheNextFr(entryIter);

        return entryIter->value;
    }

    void MoveToTheNextFr(EntryIter& entryIter) const {
        std::vector<EntryIter>& entrs = frToIters_.at(entryIter->fr);

        size_t posCurEntry = entryIter->pos;
        assert(entrs.size() > posCurEntry);

        std::swap(entrs[posCurEntry], entrs.back());
        entrs[posCurEntry]->pos = posCurEntry;
        entrs.pop_back();

        entryIter->fr = std::max(this->max_size() - 1, entryIter->fr + 1);
        std::vector<EntryIter>& nextEntrs = frToIters_[entryIter->fr];
        nextEntrs.push_back(entryIter);
        entryIter->pos = nextEntrs.size() - 1;
    }

    void PutImpl(const K& key, const V& value) override {
//        CacheIter cacheIter = cache_.find(key);

//        if (cacheIter == cache_.end()) {
//            if (entries_.size() < this->max_size()) {
//                entries_.push_front({key, value});
//            } else {
//                const K& lruKey = entries_.back().first;

//                assert(cache_.find(lruKey) != cache_.end());
//                cache_.erase(cache_.find(lruKey));

//                entries_.splice(entries_.begin(), entries_, std::prev(entries_.end()));

//                *entries_.begin() = {key, value};
//            }

//            cache_[key] = entries_.begin();
//        } else { // if I found entry that I can just modify its value
//            EntryIter entryIter = cacheIter->second;
//            entryIter->second = value;
//            entries_.splice(entries_.begin(), entries_, entryIter);
//        }
    }

private:
    std::unordered_map<K, EntryIter> cache_;
    mutable std::vector<std::vector<EntryIter>> frToIters_;
    size_t minFr_;
    mutable std::list<Entry> entries_;
};

} //namespace cache

#endif // LFUCACHE_HPP
