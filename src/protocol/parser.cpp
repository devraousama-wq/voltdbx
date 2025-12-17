#include "voltdbx/protocol.hpp"

namespace voltdbx {

std::optional<ParsedCommand> parse_line(std::string_view) {
    return std::nullopt;
}

}
