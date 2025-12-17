#include "voltdbx/net/accept_loop.hpp"

#include "voltdbx/util/logger.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>

namespace voltdbx::net {

AcceptLoop::AcceptLoop(TcpServer& server) : server_(server) {}

void AcceptLoop::set_handler(SessionHandler handler) {
    handler_ = std::move(handler);
}

std::string AcceptLoop::peer_address(int client_fd) {
    sockaddr_in peer{};
    socklen_t len = sizeof(peer);
    if (getpeername(client_fd, reinterpret_cast<sockaddr*>(&peer), &len) != 0) {
        return "unknown";
    }
    char buf[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &peer.sin_addr, buf, sizeof(buf));
    std::string addr(buf);
    addr += ":";
    addr += std::to_string(ntohs(peer.sin_port));
    return addr;
}

void AcceptLoop::run_once() {
    if (!server_.running()) {
        return;
    }
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_fd = ::accept(server_.listen_fd(),
                             reinterpret_cast<sockaddr*>(&client_addr), &len);
    if (client_fd < 0) {
        if (server_.running()) {
            util::log_warn("accept failed");
        }
        return;
    }
    auto session = std::make_unique<ClientSession>(client_fd, peer_address(client_fd));
    util::log_info("client connected id=" + std::to_string(session->id()) +
                   " peer=" + session->peer_addr());
    if (handler_) {
        handler_(std::move(session));
    } else {
        sessions_.push_back(std::move(session));
    }
}

void AcceptLoop::run_until_stopped() {
    while (server_.running()) {
        run_once();
        sessions_.erase(
            std::remove_if(sessions_.begin(), sessions_.end(),
                           [](const std::unique_ptr<ClientSession>& s) {
                               return !s->alive();
                           }),
            sessions_.end());
    }
    for (auto& s : sessions_) {
        s->mark_closed();
    }
    sessions_.clear();
}

std::size_t AcceptLoop::active_sessions() const {
    return sessions_.size();
}

}
