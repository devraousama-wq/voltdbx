#include "voltdbx/storage.hpp"

#include "voltdbx/memory/buffer_pool.hpp"
#include "voltdbx/memory/string_pool.hpp"
#include "voltdbx/storage/entry.hpp"
#include "voltdbx/storage/hash_table.hpp"
#include "voltdbx/storage/key_scanner.hpp"

namespace voltdbx {

class StorageEngine::Impl {
public:
    memory::ArenaAllocator arena{8192};
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
    if (is_expired(key)) {
        return std::nullopt;
    }
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

bool StorageEngine::set_ttl(std::string_view key, std::chrono::seconds ttl) {
    auto found = impl_->table.find(key);
    if (!found) {
        return false;
    }
    StorageEntry entry = *found;
    entry.expires_at = std::chrono::steady_clock::now() + ttl;
    impl_->table.insert(std::string(key), std::move(entry));
    return true;
}

bool StorageEngine::is_expired(std::string_view key) const {
    auto found = impl_->table.find(key);
    if (!found || !found->expires_at) {
        return false;
    }
    return std::chrono::steady_clock::now() >= *found->expires_at;
}

void StorageEngine::touch(std::string_view key) {
    auto found = impl_->table.find(key);
    if (!found || !found->expires_at) {
        return;
    }
    StorageEntry entry = *found;
    const auto remaining = *found->expires_at - std::chrono::steady_clock::now();
    entry.expires_at = std::chrono::steady_clock::now() + remaining;
    impl_->table.insert(std::string(key), std::move(entry));
}

std::size_t StorageEngine::ttl_key_count() const {
    std::size_t count = 0;
    impl_->table.for_each([&count](const std::string&, const StorageEntry& entry) {
        if (entry.expires_at) {
            ++count;
        }
    });
    return count;
}

void StorageEngine::purge_expired() {
    std::vector<std::string> expired;
    impl_->table.for_each([&](const std::string& key, const StorageEntry& entry) {
        if (entry.expires_at && std::chrono::steady_clock::now() >= *entry.expires_at) {
            expired.push_back(key);
        }
    });
    for (const auto& key : expired) {
        impl_->table.erase(key);
    }
}

ScanPage StorageEngine::scan_keys(ScanCursor cursor, std::size_t count,
                                  const std::string& pattern) const {
    KeyScanner scanner(impl_->table);
    return scanner.scan(cursor, count, pattern);
}

void StorageEngine::clear_all() {
    impl_->table.clear();
    impl_->arena.reset();
}

std::vector<std::pair<std::string, std::string>> StorageEngine::dump_entries() const {
    std::vector<std::pair<std::string, std::string>> pairs;
    impl_->table.for_each([&pairs](const std::string& key, const StorageEntry& entry) {
        pairs.emplace_back(key, entry.value);
    });
    return pairs;
}

}
