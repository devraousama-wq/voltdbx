#include "voltdbx/storage/key_scanner.hpp"

namespace voltdbx {

KeyScanner::KeyScanner(const HashTable& table) : table_(table) {}

bool KeyScanner::matches(const std::string& key, const std::string& pattern) const {
    if (pattern.empty() || pattern == "*") {
        return true;
    }
    if (pattern.front() == '*' && pattern.back() == '*' && pattern.size() > 2) {
        const std::string needle = pattern.substr(1, pattern.size() - 2);
        return key.find(needle) != std::string::npos;
    }
    if (pattern.front() == '*') {
        const std::string suffix = pattern.substr(1);
        if (suffix.size() > key.size()) {
            return false;
        }
        return key.compare(key.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    if (pattern.back() == '*') {
        const std::string prefix = pattern.substr(0, pattern.size() - 1);
        return key.rfind(prefix, 0) == 0;
    }
    return key == pattern;
}

ScanPage KeyScanner::scan(ScanCursor cursor, std::size_t count, const std::string& pattern) const {
    ScanPage page;
    page.next = cursor;
    if (count == 0) {
        page.finished = true;
        return page;
    }
    const std::size_t buckets = table_.bucket_count();
    std::uint64_t bucket = cursor.bucket;
    std::uint64_t chain = cursor.chain;
    while (bucket < buckets && page.keys.size() < count) {
        table_.for_each_bucket(bucket, [&](const std::string& key, std::uint64_t index) {
            if (index < chain) {
                return;
            }
            if (page.keys.size() >= count) {
                return;
            }
            if (matches(key, pattern)) {
                page.keys.push_back(key);
            }
            chain = index + 1;
            page.next.bucket = bucket;
            page.next.chain = chain;
        });
        if (page.keys.size() < count) {
            bucket += 1;
            chain = 0;
            page.next.bucket = bucket;
            page.next.chain = 0;
        }
    }
    page.finished = page.next.bucket >= buckets;
    return page;
}

}
