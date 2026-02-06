#pragma once

#include "voltdbx/concurrency/thread_pool.hpp"
#include "voltdbx/net/client_session.hpp"
#include "voltdbx/server/command_handler.hpp"

#include <memory>

namespace voltdbx::concurrency {

class SessionWorker {
public:
    SessionWorker(ThreadPool& pool, CommandHandler& handler);

    void dispatch(std::unique_ptr<net::ClientSession> session);

private:
    ThreadPool& pool_;
    CommandHandler& handler_;
};

}
