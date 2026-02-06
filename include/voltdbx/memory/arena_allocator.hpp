#pragma once

#include <cstddef>
#include <vector>

namespace voltdbx::memory {

class ArenaAllocator {
public:
    explicit ArenaAllocator(std::size_t block_size = 4096);

    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
    void reset();
    std::size_t bytes_allocated() const { return bytes_allocated_; }
    std::size_t block_count() const { return blocks_.size(); }

private:
    struct Block {
        std::vector<char> data;
        std::size_t offset = 0;
    };

    std::size_t block_size_;
    std::vector<Block> blocks_;
    std::size_t bytes_allocated_ = 0;
};

template <typename T>
class ArenaObjectPool {
public:
    explicit ArenaObjectPool(ArenaAllocator& arena) : arena_(arena) {}

    T* create() {
        void* mem = arena_.allocate(sizeof(T), alignof(T));
        return new (mem) T();
    }

    void reset() { arena_.reset(); }

private:
    ArenaAllocator& arena_;
};

}
