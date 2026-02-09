#pragma once

#include "voltdbx/storage.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace voltdbx {

class CounterStore {
public:
    explicit CounterStore(StorageEngine& storage);

    long long increment(std::string_view key, long long delta);
    long long decrement(std::string_view key, long long delta);
    std::optional<long long> read(std::string_view key) const;

private:
    static long long parse_value(const std::optional<std::string>& raw);
    static std::string format_value(long long value);

    StorageEngine& storage_;
};

}
