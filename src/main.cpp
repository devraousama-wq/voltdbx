#include "voltdbx/config.hpp"
#include "voltdbx/server.hpp"

#include <iostream>
#include <string>

namespace {

void print_health() {
    std::cout << "VoltDBX v0.1.0\n";
    std::cout << "status: ok\n";
}

}

int main(int argc, char** argv) {
    voltdbx::ServerConfig cfg = voltdbx::load_config_from_env();
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--health" || arg == "-h") {
            print_health();
            return 0;
        }
        if (arg == "--host" && i + 1 < argc) {
            cfg.host = argv[++i];
        } else if (arg == "--port" && i + 1 < argc) {
            cfg.port = static_cast<std::uint16_t>(std::atoi(argv[++i]));
        }
    }
    voltdbx::DatabaseServer server(cfg);
    return server.run();
}
