#pragma once

#include "voltdbx/storage.hpp"

#include <string>
#include <vector>

namespace voltdbx {

struct BatchSetPair {
    std::string key;
    std::string value;
};

class BatchOperations {
public:
    explicit BatchOperations(StorageEngine& storage);

    std::vector<std::optional<std::string>> mget(const std::vector<std::string>& keys) const;
    std::size_t mset(const std::vector<BatchSetPair>& pairs);

private:
    StorageEngine& storage_;
};

}
