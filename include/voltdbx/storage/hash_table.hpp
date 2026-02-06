#pragma once

#include "voltdbx/storage/entry.hpp"

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace voltdbx {

class HashTable {
public:
    explicit HashTable(std::size_t bucket_count = 4096);

    bool insert(std::string key, StorageEntry entry);
    std::optional<StorageEntry> find(std::string_view key) const;
    bool erase(std::string_view key);
    bool contains(std::string_view key) const;
    std::size_t size() const { return size_; }
    std::size_t bucket_count() const { return buckets_.size(); }

    void rehash_if_needed();

    template <typename Fn>
    void for_each(Fn&& fn) const;

private:
    std::size_t bucket_index(std::string_view key) const;
    static std::size_t hash_key(std::string_view key);

    struct Node {
        std::string key;
        StorageEntry entry;
        Node* next = nullptr;
    };

    std::vector<Node*> buckets_;
    std::size_t size_ = 0;
    float max_load_factor_ = 0.75f;
};

template <typename Fn>
void HashTable::for_each(Fn&& fn) const {
    for (Node* head : buckets_) {
        Node* current = head;
        while (current) {
            fn(current->key, current->entry);
            current = current->next;
        }
    }
}

}
