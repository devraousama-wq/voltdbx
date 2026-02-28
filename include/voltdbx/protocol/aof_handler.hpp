#pragma once

#include "voltdbx/persistence/aof_log.hpp"
#include "voltdbx/storage.hpp"

#include <string>

namespace voltdbx {

std::string handle_bgrewriteaof(persistence::AofLog& aof, StorageEngine& storage);

}
