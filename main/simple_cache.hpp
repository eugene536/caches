#ifndef SIMPLE_CACHE_HPP
#define SIMPLE_CACHE_HPP

#include "fixedcache.hpp"
#include <unordered_map>

namespace cache {

template <class K, class V>
class SimpleCache : public FixedCache<K, V> {
private:
    typedef FixedCache<K, V> BaseCache;

public:
    explicit SimpleCache(size_t max_size)
        : BaseCache(max_size)
    {
        cache_.reserve(max_size);
    }

protected:
    const V& GetImpl(const K& key) const override {
        return cache_.at(key);
    }

    void PutImpl(const K& key, const V& value) override {
        if (cache_.size() == this->max_size()) {
            cache_.erase(cache_.begin());
        }

        cache_[key] = value;
    }

private:
    std::unordered_map<K, V> cache_;
}; // class FixedCache

} // namespace Cache

#endif // SIMPLE_CACHE_HPP
