#pragma once

#include "voltdbx/net/accept_loop.hpp"
#include "voltdbx/net/io_multiplexer.hpp"
#include "voltdbx/net/tcp_server.hpp"

namespace voltdbx::net {

class AsyncAcceptLoop {
public:
    explicit AsyncAcceptLoop(TcpServer& server);

    void set_handler(AcceptLoop::SessionHandler handler);
    void run_until_stopped();

private:
    void handle_readable();

    TcpServer& server_;
    IoMultiplexer multiplexer_;
    AcceptLoop::SessionHandler handler_;
};

}
