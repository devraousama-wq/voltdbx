#include "voltdbx/config.hpp"

#include <cstdlib>
#include <cstring>

namespace voltdbx {

namespace {

std::string env_or(const char* key, const char* fallback) {
    const char* v = std::getenv(key);
    if (v && std::strlen(v) > 0) {
        return std::string(v);
    }
    return std::string(fallback);
}

}

ServerConfig load_config_from_env() {
    ServerConfig cfg;
    cfg.host = env_or("VOLTDBX_HOST", cfg.host.c_str());
    const char* port = std::getenv("VOLTDBX_PORT");
    if (port) {
        cfg.port = static_cast<std::uint16_t>(std::atoi(port));
    }
    const char* maxc = std::getenv("VOLTDBX_MAX_CLIENTS");
    if (maxc) {
        cfg.max_clients = static_cast<std::size_t>(std::atol(maxc));
    }
    cfg.data_dir = env_or("VOLTDBX_DATA_DIR", cfg.data_dir.c_str());
    const char* snap = std::getenv("VOLTDBX_SNAPSHOT_INTERVAL_SEC");
    if (snap) {
        cfg.snapshot_interval_sec = static_cast<std::uint32_t>(std::atoi(snap));
    }
    return cfg;
}

}
