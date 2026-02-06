#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace voltdbx {

struct StorageEntry {
    std::string value;
    std::optional<std::chrono::steady_clock::time_point> expires_at;
};

}
