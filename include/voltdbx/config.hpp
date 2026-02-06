#pragma once

#include <cstdint>
#include <string>

namespace voltdbx {

struct ServerConfig {
    std::string host = "127.0.0.1";
    std::uint16_t port = 6379;
    std::size_t max_clients = 1024;
    std::string data_dir = "./data";
    std::uint32_t snapshot_interval_sec = 300;
};

ServerConfig load_config_from_env();

}
