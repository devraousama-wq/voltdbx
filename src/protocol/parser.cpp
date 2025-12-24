#include "voltdbx/protocol.hpp"
#include "voltdbx/protocol/command.hpp"

#include <sstream>

namespace voltdbx {

namespace {

std::string trim(std::string_view s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return std::string(s.substr(start, end - start));
}

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

}

std::optional<ParsedCommand> parse_line(std::string_view raw) {
    const std::string line = trim(raw);
    if (line.empty()) {
        return std::nullopt;
    }
    std::vector<std::string> tokens = tokenize(line);
    if (tokens.empty()) {
        return std::nullopt;
    }
    ParsedCommand cmd;
    cmd.name = tokens.front();
    cmd.type = command_type_from_name(cmd.name);
    if (tokens.size() > 1) {
        cmd.args.assign(tokens.begin() + 1, tokens.end());
    }
    return cmd;
}

}
