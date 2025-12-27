#include "voltdbx/storage.hpp"

#include "voltdbx/storage/entry.hpp"
#include "voltdbx/storage/hash_table.hpp"

namespace voltdbx {

class StorageEngine::Impl {
public:
    HashTable table;
};

StorageEngine::StorageEngine() : impl_(std::make_unique<Impl>()) {}
StorageEngine::~StorageEngine() = default;

bool StorageEngine::set(std::string_view key, std::string_view value) {
    StorageEntry entry;
    entry.value = std::string(value);
    impl_->table.insert(std::string(key), std::move(entry));
    return true;
}

std::optional<std::string> StorageEngine::get(std::string_view key) const {
    auto found = impl_->table.find(key);
    if (!found) {
        return std::nullopt;
    }
    return found->value;
}

bool StorageEngine::remove(std::string_view key) {
    return impl_->table.erase(key);
}

bool StorageEngine::exists(std::string_view key) const {
    return impl_->table.contains(key);
}

std::size_t StorageEngine::entry_count() const {
    return impl_->table.size();
}

}
