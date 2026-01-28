#include "voltdbx/monitor/metrics.hpp"

#include <sstream>

namespace voltdbx::monitor {

void MetricsCollector::record_command() {
    commands_.fetch_add(1, std::memory_order_relaxed);
    std::lock_guard<std::mutex> lock(mutex_);
    ++window_commands_;
}

void MetricsCollector::set_active_clients(std::size_t count) {
    std::lock_guard<std::mutex> lock(mutex_);
    active_clients_ = count;
}

void MetricsCollector::set_keys_stored(std::size_t count) {
    std::lock_guard<std::mutex> lock(mutex_);
    keys_stored_ = count;
}

void MetricsCollector::set_memory_bytes(std::size_t bytes) {
    std::lock_guard<std::mutex> lock(mutex_);
    memory_bytes_ = bytes;
}

void MetricsCollector::tick_throughput() {
    std::lock_guard<std::mutex> lock(mutex_);
    commands_per_sec_ = static_cast<double>(window_commands_);
    window_commands_ = 0;
}

ServerMetrics MetricsCollector::snapshot() const {
    std::lock_guard<std::mutex> lock(mutex_);
    ServerMetrics metrics;
    metrics.memory_bytes = memory_bytes_;
    metrics.active_clients = active_clients_;
    metrics.commands_processed = commands_.load(std::memory_order_relaxed);
    metrics.keys_stored = keys_stored_;
    metrics.commands_per_sec = commands_per_sec_;
    return metrics;
}

std::string MetricsCollector::render_info() const {
    const ServerMetrics metrics = snapshot();
    std::ostringstream out;
    out << "+INFO voltdbx\n";
    out << "memory_bytes:" << metrics.memory_bytes << "\n";
    out << "active_clients:" << metrics.active_clients << "\n";
    out << "commands_processed:" << metrics.commands_processed << "\n";
    out << "keys_stored:" << metrics.keys_stored << "\n";
    out << "commands_per_sec:" << metrics.commands_per_sec << "\n";
    return out.str();
}

}
