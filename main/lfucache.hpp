#ifndef LFUCACHE_HPP
#define LFUCACHE_HPP

#include <memory>
#include <vector>
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
        , frToIters_(max_size)
        , minFr_(-1)
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

    void PutImpl(const K& key, const V& value) override {
        CacheIter cacheIter = cache_.find(key);

        if (cacheIter == cache_.end()) {
            if (entries_.size() < this->max_size()) {
                entries_.push_back({0, 0, key, value});
                frToIters_[0].push_back(std::prev(entries_.end()));
                entries_.back().pos = frToIters_[0].size() - 1;
                minFr_ = 0;

                cache_[key] = std::prev(entries_.end());
            } else {
                assert((int) frToIters_.size() > minFr_);
                assert(!frToIters_[minFr_].empty());

                EntryIter lfuEntry = *std::prev(frToIters_[minFr_].end());

                assert(cache_.find(lfuEntry->key) != cache_.end());
                cache_.erase(cache_.find(lfuEntry->key));
                frToIters_[minFr_].pop_back();

                *lfuEntry = {0, 0, key, value};
                cache_[key] = lfuEntry;
                frToIters_[0].push_back(lfuEntry);
                lfuEntry->pos = frToIters_[0].size() - 1;
                minFr_ = 0;
            }
        } else {
            EntryIter entryIter = cacheIter->second;
            entryIter->value = value;
            MoveToTheNextFr(entryIter);
        }
    }

    void MoveToTheNextFr(EntryIter& entryIter) const {
        if (entryIter->fr == this->max_size() - 1) return;

        std::vector<EntryIter>& entrs = frToIters_.at(entryIter->fr);

        size_t posCurEntry = entryIter->pos;
        assert(entrs.size() > posCurEntry);

        if (posCurEntry != entrs.size() - 1) {
            std::swap(entrs[posCurEntry], entrs.back());
            entrs[posCurEntry]->pos = posCurEntry;
        }

        entrs.pop_back();

        if (entrs.size() == 0 && minFr_ == (int) entryIter->fr) {
            minFr_++;

            assert(minFr_ < (int) this->max_size());
        }



        entryIter->fr = entryIter->fr + 1;
        std::vector<EntryIter>& nextEntrs = frToIters_[entryIter->fr];
        nextEntrs.push_back(entryIter);
        entryIter->pos = nextEntrs.size() - 1;

    }

private:
    std::unordered_map<K, EntryIter> cache_;
    mutable std::vector<std::vector<EntryIter>> frToIters_;
    mutable int minFr_;
    mutable std::list<Entry> entries_;
};

} //namespace cache

#endif // LFUCACHE_HPP
