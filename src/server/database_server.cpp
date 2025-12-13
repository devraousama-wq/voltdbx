#include "voltdbx/server.hpp"

namespace voltdbx {

DatabaseServer::DatabaseServer(ServerConfig config)
    : config_(std::move(config)) {}

int DatabaseServer::run() {
    return 0;
}

}
