#include "voltdbx/storage/counter_store.hpp"

#include <stdexcept>

namespace voltdbx {

CounterStore::CounterStore(StorageEngine& storage) : storage_(storage) {}

long long CounterStore::parse_value(const std::optional<std::string>& raw) {
    if (!raw || raw->empty()) {
        return 0;
    }
    return std::stoll(*raw);
}

std::string CounterStore::format_value(long long value) {
    return std::to_string(value);
}

long long CounterStore::increment(std::string_view key, long long delta) {
    const long long current = parse_value(storage_.get(key));
    const long long next = current + delta;
    storage_.set(key, format_value(next));
    return next;
}

long long CounterStore::decrement(std::string_view key, long long delta) {
    return increment(key, -delta);
}

std::optional<long long> CounterStore::read(std::string_view key) const {
    const auto raw = storage_.get(key);
    if (!raw) {
        return std::nullopt;
    }
    return parse_value(raw);
}

}
