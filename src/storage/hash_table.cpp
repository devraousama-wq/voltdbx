#include "voltdbx/storage/hash_table.hpp"

#include <functional>

namespace voltdbx {

HashTable::HashTable(std::size_t bucket_count) {
    if (bucket_count < 16) {
        bucket_count = 16;
    }
    buckets_.resize(bucket_count, nullptr);
}

std::size_t HashTable::hash_key(std::string_view key) {
    return std::hash<std::string_view>{}(key);
}

std::size_t HashTable::bucket_index(std::string_view key) const {
    return hash_key(key) % buckets_.size();
}

bool HashTable::insert(std::string key, StorageEntry entry) {
    const std::size_t idx = bucket_index(key);
    Node* current = buckets_[idx];
    while (current) {
        if (current->key == key) {
            current->entry = std::move(entry);
            return false;
        }
        current = current->next;
    }
    Node* node = new Node{std::move(key), std::move(entry), buckets_[idx]};
    buckets_[idx] = node;
    ++size_;
    rehash_if_needed();
    return true;
}

std::optional<StorageEntry> HashTable::find(std::string_view key) const {
    const std::size_t idx = bucket_index(key);
    Node* current = buckets_[idx];
    while (current) {
        if (current->key == key) {
            return current->entry;
        }
        current = current->next;
    }
    return std::nullopt;
}

bool HashTable::erase(std::string_view key) {
    const std::size_t idx = bucket_index(key);
    Node* current = buckets_[idx];
    Node* prev = nullptr;
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                buckets_[idx] = current->next;
            }
            delete current;
            --size_;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

bool HashTable::contains(std::string_view key) const {
    return find(key).has_value();
}

void HashTable::rehash_if_needed() {
    const float load = static_cast<float>(size_) / static_cast<float>(buckets_.size());
    if (load <= max_load_factor_) {
        return;
    }
    std::vector<Node*> old = std::move(buckets_);
    buckets_.clear();
    buckets_.resize(old.size() * 2, nullptr);
    size_ = 0;
    for (Node* head : old) {
        while (head) {
            Node* next = head->next;
            insert(std::move(head->key), std::move(head->entry));
            delete head;
            head = next;
        }
    }
}

void HashTable::clear() {
    for (Node* head : buckets_) {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }
    std::fill(buckets_.begin(), buckets_.end(), nullptr);
    size_ = 0;
}

}
