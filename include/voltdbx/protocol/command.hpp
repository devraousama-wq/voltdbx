#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace voltdbx {

enum class CommandType {
    Unknown,
    Ping,
    Get,
    Set,
    Delete,
    Exists,
    Expire,
    Subscribe,
    Publish,
    Info,
    Mget,
    Mset,
    Incr,
    Decr,
    Incrby,
    Scan,
    Auth
};

struct ParsedCommand {
    CommandType type = CommandType::Unknown;
    std::string name;
    std::vector<std::string> args;
};

CommandType command_type_from_name(std::string_view name);

}
