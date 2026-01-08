#include "voltdbx/ttl/expiration.hpp"

#include "voltdbx/storage/entry.hpp"

namespace voltdbx::ttl {

ExpirationService::ExpirationService(StorageEngine& storage) : storage_(storage) {}

bool ExpirationService::expire_after(std::string_view key, std::chrono::seconds ttl) {
    if (!storage_.exists(key)) {
        return false;
    }
    return storage_.set_ttl(key, ttl);
}

bool ExpirationService::is_expired(std::string_view key) const {
    return storage_.is_expired(key);
}

void ExpirationService::touch(std::string_view key) {
    storage_.touch(key);
}

std::size_t ExpirationService::active_ttl_keys() const {
    return storage_.ttl_key_count();
}

}
