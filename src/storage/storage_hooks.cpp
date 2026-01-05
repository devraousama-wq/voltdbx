#include "voltdbx/storage/storage_hooks.hpp"

namespace voltdbx {

StorageHooks::StorageHooks(StorageEngine& storage, persistence::AofLog& aof)
    : storage_(storage), aof_(aof) {}

bool StorageHooks::set(std::string_view key, std::string_view value) {
    const bool ok = storage_.set(key, value);
    if (ok) {
        aof_.append_set(key, value);
    }
    return ok;
}

bool StorageHooks::remove(std::string_view key) {
    const bool ok = storage_.remove(key);
    if (ok) {
        aof_.append_del(key);
    }
    return ok;
}

}
