#include "voltdbx/storage.hpp"

#include "voltdbx/memory/buffer_pool.hpp"
#include "voltdbx/memory/string_pool.hpp"
#include "voltdbx/storage/entry.hpp"
#include "voltdbx/storage/hash_table.hpp"

namespace voltdbx {

class StorageEngine::Impl {
public:
    memory::BufferPool buffer_pool{512, 256};
    memory::StringPool string_pool{buffer_pool};
    HashTable table;
};

StorageEngine::StorageEngine() : impl_(std::make_unique<Impl>()) {}
StorageEngine::~StorageEngine() = default;

bool StorageEngine::set(std::string_view key, std::string_view value) {
    StorageEntry entry;
    entry.value = impl_->string_pool.copy(value);
    impl_->table.insert(impl_->string_pool.copy(key), std::move(entry));
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
