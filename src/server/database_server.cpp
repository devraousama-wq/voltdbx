#include "voltdbx/server.hpp"
#include "voltdbx/net/tcp_server.hpp"
#include "voltdbx/util/logger.hpp"

namespace voltdbx {

DatabaseServer::DatabaseServer(ServerConfig config)
    : config_(std::move(config)) {}

int DatabaseServer::run() {
    net::TcpServer tcp(config_.host, config_.port);
    tcp.install_signal_handlers(
        [](int) { util::log_info("received SIGINT, shutting down"); },
        [](int) { util::log_info("received SIGTERM, shutting down"); });
    if (!tcp.bind_and_listen()) {
        util::log_error("failed to start TCP listener");
        return 1;
    }
    util::log_info("TCP server bound, awaiting accept loop");
    while (tcp.running()) {
    }
    util::log_info("server stopped cleanly");
    return 0;
}

}
