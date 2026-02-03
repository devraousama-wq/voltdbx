#include "voltdbx/bench/latency_runner.hpp"
#include "voltdbx/storage.hpp"

#include <iostream>

int main() {
    voltdbx::StorageEngine storage;
    voltdbx::bench::LatencyRunner runner(storage);
    const auto set_get = runner.run_set_get(5000);
    const auto exists = runner.run_exists(5000);
    std::cout << "set/get rps=" << set_get.requests_per_sec << " p50=" << set_get.p50_ms
              << " p95=" << set_get.p95_ms << " p99=" << set_get.p99_ms << "\n";
    std::cout << "exists rps=" << exists.requests_per_sec << "\n";
    return 0;
}
