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
    util::log_info("TCP server bound, starting accept loop");
    net::AcceptLoop accept_loop(tcp);
    accept_loop.set_handler([](std::unique_ptr<net::ClientSession> session) {
        session->write_line("+OK connected");
        std::string line = session->read_line();
        if (!line.empty()) {
            session->write_line("+PONG " + line);
        }
        session->mark_closed();
    });
    accept_loop.run_until_stopped();
    util::log_info("server stopped cleanly");
    return 0;
}

}
