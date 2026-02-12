#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/storage.hpp"

#include <string>

namespace voltdbx {

ScanCursor decode_scan_cursor(const std::string& token);
std::string encode_scan_cursor(const ScanCursor& cursor);
std::string handle_scan(const ParsedCommand& cmd, StorageEngine& storage);

}
