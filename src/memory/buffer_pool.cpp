#include "voltdbx/memory/buffer_pool.hpp"

namespace voltdbx::memory {

BufferPool::BufferPool(std::size_t block_size, std::size_t initial_blocks)
    : block_size_(block_size) {
    slabs_.emplace_back(initial_blocks * block_size_);
    char* base = slabs_.back().data();
    for (std::size_t i = 0; i < initial_blocks; ++i) {
        free_list_.push_back(base + i * block_size_);
    }
}

char* BufferPool::acquire(std::size_t min_size) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (min_size > block_size_) {
        slabs_.emplace_back(min_size);
        return slabs_.back().data();
    }
    if (free_list_.empty()) {
        slabs_.emplace_back(block_size_ * 64);
        char* base = slabs_.back().data();
        for (std::size_t i = 0; i < 64; ++i) {
            free_list_.push_back(base + i * block_size_);
        }
    }
    char* block = free_list_.back();
    free_list_.pop_back();
    return block;
}

void BufferPool::release(char* block) {
    if (!block) {
        return;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    free_list_.push_back(block);
}

std::size_t BufferPool::allocated_blocks() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::size_t total = 0;
    for (const auto& slab : slabs_) {
        total += slab.size() / block_size_;
    }
    return total;
}

std::size_t BufferPool::free_blocks() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return free_list_.size();
}

}
