#include "voltdbx/storage.hpp"

#include "voltdbx/storage/entry.hpp"

#include <unordered_map>

namespace voltdbx {

class StorageEngine::Impl {
public:
    std::unordered_map<std::string, StorageEntry> data;
};

StorageEngine::StorageEngine() : impl_(std::make_unique<Impl>()) {}
StorageEngine::~StorageEngine() = default;

bool StorageEngine::set(std::string_view key, std::string_view value) {
    StorageEntry entry;
    entry.value = std::string(value);
    impl_->data[std::string(key)] = std::move(entry);
    return true;
}

std::optional<std::string> StorageEngine::get(std::string_view key) const {
    auto it = impl_->data.find(std::string(key));
    if (it == impl_->data.end()) {
        return std::nullopt;
    }
    return it->second.value;
}

bool StorageEngine::remove(std::string_view key) {
    return impl_->data.erase(std::string(key)) > 0;
}

bool StorageEngine::exists(std::string_view key) const {
    return impl_->data.find(std::string(key)) != impl_->data.end();
}

}
