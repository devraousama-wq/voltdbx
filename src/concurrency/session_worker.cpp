#include "voltdbx/concurrency/session_worker.hpp"

namespace voltdbx::concurrency {

SessionWorker::SessionWorker(ThreadPool& pool, CommandHandler& handler)
    : pool_(pool), handler_(handler) {}

void SessionWorker::dispatch(std::unique_ptr<net::ClientSession> session) {
    pool_.enqueue([this, session = std::move(session)]() mutable {
        session->write_line("+OK voltdbx ready");
        handler_.handle_session(*session);
    });
}

}
