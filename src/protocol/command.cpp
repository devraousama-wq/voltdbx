#include "voltdbx/protocol/command.hpp"

#include <algorithm>
#include <cctype>

namespace voltdbx {

namespace {

std::string upper(std::string_view s) {
    std::string out(s);
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return out;
}

}

CommandType command_type_from_name(std::string_view name) {
    const std::string key = upper(name);
    if (key == "PING") return CommandType::Ping;
    if (key == "GET") return CommandType::Get;
    if (key == "SET") return CommandType::Set;
    if (key == "DEL" || key == "DELETE") return CommandType::Delete;
    if (key == "EXISTS") return CommandType::Exists;
    if (key == "EXPIRE") return CommandType::Expire;
    if (key == "SUBSCRIBE") return CommandType::Subscribe;
    if (key == "PUBLISH") return CommandType::Publish;
    if (key == "INFO") return CommandType::Info;
    if (key == "MGET") return CommandType::Mget;
    if (key == "MSET") return CommandType::Mset;
    if (key == "INCR") return CommandType::Incr;
    if (key == "DECR") return CommandType::Decr;
    if (key == "INCRBY") return CommandType::Incrby;
    if (key == "SCAN") return CommandType::Scan;
    if (key == "AUTH") return CommandType::Auth;
    if (key == "CONFIG") return CommandType::Config;
    return CommandType::Unknown;
}

}
