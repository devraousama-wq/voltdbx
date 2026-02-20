#pragma once

#include "voltdbx/config/runtime_config.hpp"
#include "voltdbx/protocol/command.hpp"

#include <string>

namespace voltdbx {

std::string handle_config(const ParsedCommand& cmd, RuntimeConfig& runtime);

}
