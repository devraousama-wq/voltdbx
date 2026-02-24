#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/storage.hpp"

#include <string>

namespace voltdbx {

std::string handle_flushdb(StorageEngine& storage);
std::string handle_dbsize(const StorageEngine& storage);

}
