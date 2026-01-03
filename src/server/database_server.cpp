#include "voltdbx/server.hpp"
#include "voltdbx/net/accept_loop.hpp"
#include "voltdbx/net/tcp_server.hpp"
#include "voltdbx/persistence/snapshot.hpp"
#include "voltdbx/persistence/snapshot_scheduler.hpp"
#include "voltdbx/server/command_handler.hpp"
#include "voltdbx/util/logger.hpp"

namespace voltdbx {

DatabaseServer::DatabaseServer(ServerConfig config)
    : config_(std::move(config)),
      dispatcher_(std::make_unique<CommandDispatcher>(storage_)) {}

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
    CommandHandler commands(storage_, *dispatcher_);
    accept_loop.set_handler([&commands](std::unique_ptr<net::ClientSession> session) {
        session->write_line("+OK voltdbx ready");
        commands.handle_session(*session);
    });
    accept_loop.run_until_stopped();
    util::log_info("server stopped cleanly");
    return 0;
}

}
