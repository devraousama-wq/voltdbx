#pragma once

#include <chrono>
#include <cstddef>
#include <mutex>
#include <unordered_map>

namespace voltdbx::net {

struct ConnectionStats {
    std::size_t active = 0;
    std::size_t rejected = 0;
    std::size_t idle_closed = 0;
};

class ConnectionPool {
public:
    explicit ConnectionPool(std::size_t max_connections,
                            std::chrono::seconds idle_timeout);

    bool admit();
    void release();
    void tick_idle_cleanup();
    ConnectionStats stats() const;

private:
    std::size_t max_connections_;
    std::chrono::seconds idle_timeout_;
    mutable std::mutex mutex_;
    std::size_t active_ = 0;
    std::size_t rejected_ = 0;
    std::size_t idle_closed_ = 0;
    std::chrono::steady_clock::time_point last_activity_;
};

}
