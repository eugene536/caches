#ifndef FIXEDCACHE_H
#define FIXEDCACHE_H
#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>

namespace cache {

enum Policy {
    LRU,
    LFU,
    FIFO
};

template <class V>
class CacheEntry {
public:
    explicit CacheEntry(const V& value)
        : value_(value)
    {}

    virtual ~CacheEntry() {}

    void setValue(const V& value) {
        value_ = value;
    }

    const V& value() const {
        return value_;
    }

protected:
    V value_;
};

template <class K, class V>
class FixedCache {
public:
    explicit FixedCache(size_t max_size)
        : max_size_(max_size)
    {
        cache_.reserve(max_size);
    }

    virtual ~FixedCache() {}

    FixedCache(const FixedCache&) = delete;

    FixedCache& operator=(const FixedCache<K, V>&) = delete;

    size_t max_size() const {
        return max_size_;
    }

    const V& Get(const K& key) const {
        std::lock_guard<std::mutex> guard(cache_mutex_);

        return GetImpl(key);
    }

    void Put(const K& key, const V& value) {
        std::lock_guard<std::mutex> guard(cache_mutex_);

        PutImpl(key, value);
    }

protected:
    virtual const V& GetImpl(const K& key) const {
        return cache_.at(key)->value();
    }

    virtual void PutImpl(const K& key, const V& value) {
        if (cache_.size() == max_size_) {
            cache_.erase(cache_.begin());
        }

        cache_[key] = std::make_shared<CacheEntry<V>>(value);
    }

private:
    const size_t max_size_;
    std::unordered_map<K, std::shared_ptr<CacheEntry<V>>> cache_;
    mutable std::mutex cache_mutex_;
}; // class FixedCache

} // namespace Cache

#endif // FIXEDCACHE_H
