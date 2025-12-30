#include "voltdbx/memory/string_pool.hpp"

#include <cstring>

namespace voltdbx::memory {

StringPool::StringPool(BufferPool& pool) : pool_(pool) {}

std::string StringPool::copy(std::string_view source) {
    ++copies_;
    if (source.size() <= pool_.block_size()) {
        char* block = pool_.acquire(source.size());
        std::memcpy(block, source.data(), source.size());
        std::string result(block, source.size());
        pool_.release(block);
        return result;
    }
    return std::string(source);
}

void StringPool::reset_stats() {
    copies_ = 0;
}

}
