#pragma once

#include "voltdbx/memory/buffer_pool.hpp"

#include <string>
#include <string_view>

namespace voltdbx::memory {

class StringPool {
public:
    explicit StringPool(BufferPool& pool);

    std::string copy(std::string_view source);
    void reset_stats();
    std::size_t copies() const { return copies_; }

private:
    BufferPool& pool_;
    std::size_t copies_ = 0;
};

}
