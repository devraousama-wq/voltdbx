#include "voltdbx/net/tcp_server.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace voltdbx::net {

namespace {

TcpServer* g_active_server = nullptr;

void forward_signal(int sig) {
    if (g_active_server) {
        g_active_server->stop();
    }
    std::signal(sig, SIG_DFL);
}

}

TcpServer::TcpServer(std::string host, std::uint16_t port)
    : host_(std::move(host)), port_(port) {}

TcpServer::~TcpServer() {
    stop();
    if (g_active_server == this) {
        g_active_server = nullptr;
    }
}

bool TcpServer::bind_and_listen() {
    listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) {
        std::cerr << "socket failed: " << std::strerror(errno) << "\n";
        return false;
    }

    int reuse = 1;
    if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt SO_REUSEADDR failed\n";
        close_listen();
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);

    if (host_ == "0.0.0.0" || host_.empty()) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else if (inet_pton(AF_INET, host_.c_str(), &addr.sin_addr) != 1) {
        std::cerr << "invalid bind address: " << host_ << "\n";
        close_listen();
        return false;
    }

    if (::bind(listen_fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "bind failed on " << host_ << ":" << port_ << ": "
                  << std::strerror(errno) << "\n";
        close_listen();
        return false;
    }

    if (::listen(listen_fd_, SOMAXCONN) < 0) {
        std::cerr << "listen failed: " << std::strerror(errno) << "\n";
        close_listen();
        return false;
    }

    running_.store(true);
    g_active_server = this;
    setup_signals();
    return true;
}

void TcpServer::stop() {
    if (!running_.exchange(false)) {
        return;
    }
    close_listen();
}

bool TcpServer::running() const {
    return running_.load();
}

void TcpServer::close_listen() {
    if (listen_fd_ >= 0) {
        ::close(listen_fd_);
        listen_fd_ = -1;
    }
}

void TcpServer::install_signal_handlers(SignalHandler on_sigint, SignalHandler on_sigterm) {
    on_sigint_ = std::move(on_sigint);
    on_sigterm_ = std::move(on_sigterm);
}

void TcpServer::setup_signals() {
    std::signal(SIGINT, [](int sig) {
        forward_signal(sig);
        if (g_active_server && g_active_server->on_sigint_) {
            g_active_server->on_sigint_(sig);
        }
    });
    std::signal(SIGTERM, [](int sig) {
        forward_signal(sig);
        if (g_active_server && g_active_server->on_sigterm_) {
            g_active_server->on_sigterm_(sig);
        }
    });
}

}
