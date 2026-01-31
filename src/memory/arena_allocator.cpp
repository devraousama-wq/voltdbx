#include "voltdbx/memory/arena_allocator.hpp"

#include <cstring>

namespace voltdbx::memory {

ArenaAllocator::ArenaAllocator(std::size_t block_size) : block_size_(block_size) {
    blocks_.push_back(Block{std::vector<char>(block_size_), 0});
}

void* ArenaAllocator::allocate(std::size_t size, std::size_t alignment) {
    if (blocks_.empty()) {
        blocks_.push_back(Block{std::vector<char>(block_size_), 0});
    }
    Block& block = blocks_.back();
    std::size_t aligned = (block.offset + alignment - 1) & ~(alignment - 1);
    if (aligned + size > block.data.size()) {
        blocks_.push_back(Block{std::vector<char>(std::max(block_size_, size)), 0});
        return allocate(size, alignment);
    }
    void* ptr = block.data.data() + aligned;
    block.offset = aligned + size;
    bytes_allocated_ += size;
    return ptr;
}

void ArenaAllocator::reset() {
    for (auto& block : blocks_) {
        block.offset = 0;
    }
    bytes_allocated_ = 0;
}

}
