#pragma once

#include <cstddef>
#include <mutex>
#include <vector>

namespace voltdbx::memory {

class BufferPool {
public:
    explicit BufferPool(std::size_t block_size = 256, std::size_t initial_blocks = 128);

    char* acquire(std::size_t min_size);
    void release(char* block);

    std::size_t block_size() const { return block_size_; }
    std::size_t allocated_blocks() const;
    std::size_t free_blocks() const;

private:
    std::size_t block_size_;
    std::vector<char*> free_list_;
    std::vector<std::vector<char>> slabs_;
    mutable std::mutex mutex_;
};

}
