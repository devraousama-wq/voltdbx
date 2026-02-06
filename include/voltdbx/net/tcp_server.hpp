#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <string>

namespace voltdbx::net {

class TcpServer {
public:
    using SignalHandler = std::function<void(int)>;

    TcpServer(std::string host, std::uint16_t port);
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    bool bind_and_listen();
    void stop();
    bool running() const;

    int listen_fd() const { return listen_fd_; }

    void install_signal_handlers(SignalHandler on_sigint, SignalHandler on_sigterm);

private:
    void close_listen();
    void setup_signals();

    std::string host_;
    std::uint16_t port_;
    int listen_fd_ = -1;
    std::atomic<bool> running_{false};
    SignalHandler on_sigint_;
    SignalHandler on_sigterm_;
};

}
