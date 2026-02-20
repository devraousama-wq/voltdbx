#include "voltdbx/config/runtime_config.hpp"

#include <sstream>

namespace voltdbx {

RuntimeConfig::RuntimeConfig(ServerConfig defaults) : defaults_(std::move(defaults)) {}

std::optional<std::string> RuntimeConfig::get(std::string_view key) const {
    const std::string lookup(key);
    auto it = overrides_.find(lookup);
    if (it != overrides_.end()) {
        return it->second;
    }
    if (lookup == "host") {
        return defaults_.host;
    }
    if (lookup == "port") {
        return std::to_string(defaults_.port);
    }
    if (lookup == "max_clients") {
        return std::to_string(defaults_.max_clients);
    }
    if (lookup == "data_dir") {
        return defaults_.data_dir;
    }
    if (lookup == "snapshot_interval_sec") {
        return std::to_string(defaults_.snapshot_interval_sec);
    }
    if (lookup == "requirepass") {
        return defaults_.requirepass;
    }
    return std::nullopt;
}

bool RuntimeConfig::set(std::string_view key, std::string_view value) {
    if (key == "requirepass" || key == "data_dir" || key == "host") {
        overrides_[std::string(key)] = std::string(value);
        return true;
    }
    if (key == "max_clients" || key == "port" || key == "snapshot_interval_sec") {
        overrides_[std::string(key)] = std::string(value);
        return true;
    }
    return false;
}

std::string RuntimeConfig::dump() const {
    std::ostringstream out;
    out << "host=" << *get("host") << "\n";
    out << "port=" << *get("port") << "\n";
    out << "max_clients=" << *get("max_clients") << "\n";
    out << "data_dir=" << *get("data_dir") << "\n";
    out << "snapshot_interval_sec=" << *get("snapshot_interval_sec") << "\n";
    out << "requirepass=" << *get("requirepass") << "\n";
    return out.str();
}

}
