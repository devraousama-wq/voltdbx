#pragma once

#include "voltdbx/storage.hpp"

#include <chrono>
#include <string>
#include <string_view>

namespace voltdbx::ttl {

class ExpirationService {
public:
    explicit ExpirationService(StorageEngine& storage);

    bool expire_after(std::string_view key, std::chrono::seconds ttl);
    bool is_expired(std::string_view key) const;
    void touch(std::string_view key);
    std::size_t active_ttl_keys() const;

private:
    StorageEngine& storage_;
};

}
