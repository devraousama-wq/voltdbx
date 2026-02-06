#pragma once

#include <atomic>
#include <cstddef>
#include <mutex>
#include <string>

namespace voltdbx::monitor {

struct ServerMetrics {
    std::size_t memory_bytes = 0;
    std::size_t active_clients = 0;
    std::size_t commands_processed = 0;
    std::size_t keys_stored = 0;
    double commands_per_sec = 0.0;
};

class MetricsCollector {
public:
    void record_command();
    void set_active_clients(std::size_t count);
    void set_keys_stored(std::size_t count);
    void set_memory_bytes(std::size_t bytes);
    void tick_throughput();
    ServerMetrics snapshot() const;
    std::string render_info() const;

private:
    mutable std::mutex mutex_;
    std::atomic<std::size_t> commands_{0};
    std::size_t active_clients_ = 0;
    std::size_t keys_stored_ = 0;
    std::size_t memory_bytes_ = 0;
    std::size_t window_commands_ = 0;
    double commands_per_sec_ = 0.0;
};

}
