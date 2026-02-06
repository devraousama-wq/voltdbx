#pragma once

#include "voltdbx/storage.hpp"

#include <chrono>
#include <cstddef>
#include <vector>

namespace voltdbx::bench {

struct BenchResult {
    double requests_per_sec = 0.0;
    double p50_ms = 0.0;
    double p95_ms = 0.0;
    double p99_ms = 0.0;
    std::size_t operations = 0;
};

class LatencyRunner {
public:
    explicit LatencyRunner(StorageEngine& storage);

    BenchResult run_set_get(std::size_t iterations);
    BenchResult run_exists(std::size_t iterations);

private:
    static double percentile(std::vector<double>& samples, double p);

    StorageEngine& storage_;
};

}
