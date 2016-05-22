#ifndef FIXEDCACHE_H
#define FIXEDCACHE_H
#include <memory>
#include <thread>
#include <mutex>
#include <cassert>

namespace cache {

template <class K, class V>
class FixedCache {
public:
    explicit FixedCache(size_t max_size)
        : max_size_(max_size)
    {
        assert(max_size > 0);
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
    virtual const V& GetImpl(const K& key) const = 0;

    virtual void PutImpl(const K& key, const V& value) = 0;

private:
    mutable std::mutex cache_mutex_;
    const size_t max_size_;
}; // class FixedCache

} // namespace Cache

#endif // FIXEDCACHE_H
