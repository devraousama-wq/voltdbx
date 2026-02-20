#pragma once

#include "voltdbx/config.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace voltdbx {

class RuntimeConfig {
public:
    explicit RuntimeConfig(ServerConfig defaults);

    std::optional<std::string> get(std::string_view key) const;
    bool set(std::string_view key, std::string_view value);
    std::string dump() const;

private:
    ServerConfig defaults_;
    std::unordered_map<std::string, std::string> overrides_;
};

}
