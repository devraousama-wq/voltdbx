#pragma once

#include "voltdbx/net/client_session.hpp"
#include "voltdbx/net/tcp_server.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace voltdbx::net {

class AcceptLoop {
public:
    using SessionHandler = std::function<void(std::unique_ptr<ClientSession>)>;

    explicit AcceptLoop(TcpServer& server);

    void set_handler(SessionHandler handler);
    void run_once();
    void run_until_stopped();
    std::size_t active_sessions() const;

private:
    std::string peer_address(int client_fd);

    TcpServer& server_;
    SessionHandler handler_;
    std::vector<std::unique_ptr<ClientSession>> sessions_;
};

}
