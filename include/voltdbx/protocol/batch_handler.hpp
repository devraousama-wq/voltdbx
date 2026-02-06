#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/storage/batch_ops.hpp"

#include <string>

namespace voltdbx {

std::string handle_mget(const ParsedCommand& cmd, BatchOperations& batch);
std::string handle_mset(const ParsedCommand& cmd, BatchOperations& batch);

}
