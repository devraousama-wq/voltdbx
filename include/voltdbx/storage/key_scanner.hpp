#pragma once

#include "voltdbx/storage/hash_table.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace voltdbx {

struct ScanCursor {
    std::uint64_t bucket = 0;
    std::uint64_t chain = 0;
};

struct ScanPage {
    ScanCursor next;
    std::vector<std::string> keys;
    bool finished = false;
};

class KeyScanner {
public:
    explicit KeyScanner(const HashTable& table);

    ScanPage scan(ScanCursor cursor, std::size_t count, const std::string& pattern) const;

private:
    bool matches(const std::string& key, const std::string& pattern) const;

    const HashTable& table_;
};

}
