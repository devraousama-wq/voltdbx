#pragma once

#include "voltdbx/storage/key_scanner.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace voltdbx {

class StorageEngine {
public:
    StorageEngine();
    ~StorageEngine();

    StorageEngine(const StorageEngine&) = delete;
    StorageEngine& operator=(const StorageEngine&) = delete;

    bool set(std::string_view key, std::string_view value);
    std::optional<std::string> get(std::string_view key) const;
    bool remove(std::string_view key);
    bool exists(std::string_view key) const;
    std::size_t entry_count() const;
    std::vector<std::pair<std::string, std::string>> dump_entries() const;
    bool set_ttl(std::string_view key, std::chrono::seconds ttl);
    bool is_expired(std::string_view key) const;
    void touch(std::string_view key);
    std::size_t ttl_key_count() const;
    void purge_expired();
    ScanPage scan_keys(ScanCursor cursor, std::size_t count, const std::string& pattern) const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
