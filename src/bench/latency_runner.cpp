#include "voltdbx/bench/latency_runner.hpp"

#include <algorithm>
#include <sstream>

namespace voltdbx::bench {

LatencyRunner::LatencyRunner(StorageEngine& storage) : storage_(storage) {}

double LatencyRunner::percentile(std::vector<double>& samples, double p) {
    if (samples.empty()) {
        return 0.0;
    }
    std::sort(samples.begin(), samples.end());
    const std::size_t index = static_cast<std::size_t>(p * static_cast<double>(samples.size() - 1));
    return samples[index];
}

BenchResult LatencyRunner::run_set_get(std::size_t iterations) {
    std::vector<double> latencies;
    latencies.reserve(iterations);
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < iterations; ++i) {
        const auto op_start = std::chrono::steady_clock::now();
        const std::string key = "bench:key:" + std::to_string(i);
        storage_.set(key, "payload");
        storage_.get(key);
        const auto op_end = std::chrono::steady_clock::now();
        const double ms = std::chrono::duration<double, std::milli>(op_end - op_start).count();
        latencies.push_back(ms);
    }
    const auto end = std::chrono::steady_clock::now();
    const double elapsed = std::chrono::duration<double>(end - start).count();
    BenchResult result;
    result.operations = iterations * 2;
    result.requests_per_sec = static_cast<double>(result.operations) / elapsed;
    result.p50_ms = percentile(latencies, 0.50);
    result.p95_ms = percentile(latencies, 0.95);
    result.p99_ms = percentile(latencies, 0.99);
    return result;
}

BenchResult LatencyRunner::run_exists(std::size_t iterations) {
    for (std::size_t i = 0; i < iterations; ++i) {
        storage_.set("exists:" + std::to_string(i), "1");
    }
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < iterations; ++i) {
        storage_.exists("exists:" + std::to_string(i));
    }
    const auto end = std::chrono::steady_clock::now();
    const double elapsed = std::chrono::duration<double>(end - start).count();
    BenchResult result;
    result.operations = iterations;
    result.requests_per_sec = static_cast<double>(iterations) / elapsed;
    return result;
}

}
