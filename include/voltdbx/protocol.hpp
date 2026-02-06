#pragma once

#include "voltdbx/protocol/command.hpp"

#include <optional>
#include <string_view>

namespace voltdbx {

std::optional<ParsedCommand> parse_line(std::string_view line);

}
