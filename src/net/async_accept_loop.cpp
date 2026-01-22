#include "voltdbx/net/async_accept_loop.hpp"

#include "voltdbx/net/client_session.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

namespace voltdbx::net {

AsyncAcceptLoop::AsyncAcceptLoop(TcpServer& server) : server_(server) {}

void AsyncAcceptLoop::set_handler(AcceptLoop::SessionHandler handler) {
    handler_ = std::move(handler);
}

void AsyncAcceptLoop::handle_readable() {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_fd = ::accept(server_.listen_fd(),
                             reinterpret_cast<sockaddr*>(&client_addr), &len);
    if (client_fd < 0) {
        return;
    }
    char buf[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &client_addr.sin_addr, buf, sizeof(buf));
    std::string peer = std::string(buf) + ":" + std::to_string(ntohs(client_addr.sin_port));
    auto session = std::make_unique<ClientSession>(client_fd, peer);
    if (handler_) {
        handler_(std::move(session));
    }
}

void AsyncAcceptLoop::run_until_stopped() {
    multiplexer_.add_read(server_.listen_fd());
    multiplexer_.set_readable_callback([this](int fd) {
        if (fd == server_.listen_fd()) {
            handle_readable();
        }
    });
    while (server_.running()) {
        std::vector<int> ready;
        multiplexer_.wait(ready, 250);
    }
}

}
