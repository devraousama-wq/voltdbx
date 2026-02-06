#include "voltdbx/storage/batch_ops.hpp"

namespace voltdbx {

BatchOperations::BatchOperations(StorageEngine& storage) : storage_(storage) {}

std::vector<std::optional<std::string>> BatchOperations::mget(
    const std::vector<std::string>& keys) const {
    std::vector<std::optional<std::string>> values;
    values.reserve(keys.size());
    for (const auto& key : keys) {
        values.push_back(storage_.get(key));
    }
    return values;
}

std::size_t BatchOperations::mset(const std::vector<BatchSetPair>& pairs) {
    std::size_t written = 0;
    for (const auto& pair : pairs) {
        if (storage_.set(pair.key, pair.value)) {
            ++written;
        }
    }
    return written;
}

}
