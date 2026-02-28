#include "voltdbx/server.hpp"
#include "voltdbx/net/async_accept_loop.hpp"
#include "voltdbx/net/connection_pool.hpp"
#include "voltdbx/net/tcp_server.hpp"
#include "voltdbx/persistence/snapshot.hpp"
#include "voltdbx/ttl/expiration.hpp"
#include "voltdbx/ttl/scheduler.hpp"
#include "voltdbx/persistence/snapshot_scheduler.hpp"
#include "voltdbx/concurrency/session_worker.hpp"
#include "voltdbx/concurrency/thread_pool.hpp"
#include "voltdbx/server/command_handler.hpp"
#include "voltdbx/util/logger.hpp"

namespace voltdbx {

DatabaseServer::DatabaseServer(ServerConfig config)
    : config_(std::move(config)),
      aof_(config_.data_dir),
      broker_(channels_),
      auth_(config_.requirepass),
      runtime_(config_),
      dispatcher_(std::make_unique<CommandDispatcher>(storage_, broker_, metrics_, auth_, runtime_,
                                                        aof_)) {}

int DatabaseServer::run() {
    net::TcpServer tcp(config_.host, config_.port);
    tcp.install_signal_handlers(
        [](int) { util::log_info("received SIGINT, shutting down"); },
        [](int) { util::log_info("received SIGTERM, shutting down"); });
    persistence::SnapshotLoader loader(config_.data_dir);
    if (loader.has_snapshot()) {
        loader.load_into(storage_);
        util::log_info("restored data from snapshot");
    } else {
        aof_.replay(storage_);
        util::log_info("replayed append-only log");
    }
    persistence::SnapshotWriter writer(config_.data_dir);
    persistence::SnapshotScheduler snapshots(storage_, writer, config_.snapshot_interval_sec);
    ttl::ExpirationService expiration(storage_);
    ttl::ExpirationScheduler ttl_scheduler(storage_, expiration, std::chrono::milliseconds(500));
    snapshots.start();
    ttl_scheduler.start();
    if (!tcp.bind_and_listen()) {
        util::log_error("failed to start TCP listener");
        ttl_scheduler.stop();
        snapshots.stop();
        return 1;
    }
    util::log_info("TCP server bound, starting accept loop");
    net::ConnectionPool connections(config_.max_clients, std::chrono::seconds(120));
    net::AsyncAcceptLoop accept_loop(tcp);
    CommandHandler commands(storage_, *dispatcher_);
    concurrency::ThreadPool pool(4);
    concurrency::SessionWorker workers(pool, commands);
    accept_loop.set_handler([&workers, &connections, this](std::unique_ptr<net::ClientSession> session) {
        if (!connections.admit()) {
            session->write_line("-ERR max clients reached");
            session->mark_closed();
            return;
        }
        metrics_.set_active_clients(connections.stats().active + 1);
        workers.dispatch(std::move(session));
        connections.release();
        metrics_.set_active_clients(connections.stats().active);
        metrics_.set_keys_stored(storage_.entry_count());
        metrics_.set_memory_bytes(storage_.entry_count() * 128);
    });
    accept_loop.run_until_stopped();
    pool.shutdown();
    ttl_scheduler.stop();
    snapshots.stop();
    writer.write(storage_);
    util::log_info("server stopped cleanly");
    return 0;
}

}
