#pragma once

#include "voltdbx/persistence/aof_log.hpp"
#include "voltdbx/storage.hpp"

namespace voltdbx {

class StorageHooks {
public:
    StorageHooks(StorageEngine& storage, persistence::AofLog& aof);

    bool set(std::string_view key, std::string_view value);
    bool remove(std::string_view key);

private:
    StorageEngine& storage_;
    persistence::AofLog& aof_;
};

}
