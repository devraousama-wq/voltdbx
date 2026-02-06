#pragma once

#include <cstdint>
#include <string>

namespace voltdbx::net {

class ClientSession {
public:
    ClientSession(int fd, std::string peer_addr);
    ~ClientSession();

    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;

    int fd() const { return fd_; }
    const std::string& peer_addr() const { return peer_addr_; }
    std::uint64_t id() const { return id_; }
    bool alive() const { return alive_; }
    void mark_closed();

    std::string read_line();
    bool write_line(const std::string& line);

private:
    static std::uint64_t next_id();

    int fd_;
    std::string peer_addr_;
    std::uint64_t id_;
    bool alive_ = true;
};

}
