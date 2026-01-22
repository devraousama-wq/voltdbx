#include "voltdbx/net/connection_pool.hpp"

namespace voltdbx::net {

ConnectionPool::ConnectionPool(std::size_t max_connections, std::chrono::seconds idle_timeout)
    : max_connections_(max_connections),
      idle_timeout_(idle_timeout),
      last_activity_(std::chrono::steady_clock::now()) {}

bool ConnectionPool::admit() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (active_ >= max_connections_) {
        ++rejected_;
        return false;
    }
    ++active_;
    last_activity_ = std::chrono::steady_clock::now();
    return true;
}

void ConnectionPool::release() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (active_ > 0) {
        --active_;
    }
    last_activity_ = std::chrono::steady_clock::now();
}

void ConnectionPool::tick_idle_cleanup() {
    std::lock_guard<std::mutex> lock(mutex_);
    const auto now = std::chrono::steady_clock::now();
    if (active_ == 0 && now - last_activity_ > idle_timeout_) {
        ++idle_closed_;
        last_activity_ = now;
    }
}

ConnectionStats ConnectionPool::stats() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return ConnectionStats{active_, rejected_, idle_closed_};
}

}
