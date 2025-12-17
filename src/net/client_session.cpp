#include "voltdbx/net/client_session.hpp"

#include <unistd.h>

#include <cstring>
#include <sstream>

namespace voltdbx::net {

namespace {

std::uint64_t g_session_counter = 1;

}

std::uint64_t ClientSession::next_id() {
    return g_session_counter++;
}

ClientSession::ClientSession(int fd, std::string peer_addr)
    : fd_(fd), peer_addr_(std::move(peer_addr)), id_(next_id()) {}

ClientSession::~ClientSession() {
    mark_closed();
}

void ClientSession::mark_closed() {
    if (!alive_) {
        return;
    }
    alive_ = false;
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

std::string ClientSession::read_line() {
    std::string buffer;
    char ch = 0;
    while (alive_) {
        ssize_t n = ::read(fd_, &ch, 1);
        if (n <= 0) {
            mark_closed();
            break;
        }
        if (ch == '\n') {
            if (!buffer.empty() && buffer.back() == '\r') {
                buffer.pop_back();
            }
            break;
        }
        buffer.push_back(ch);
        if (buffer.size() > 65536) {
            mark_closed();
            break;
        }
    }
    return buffer;
}

bool ClientSession::write_line(const std::string& line) {
    if (!alive_) {
        return false;
    }
    std::string payload = line;
    if (payload.empty() || payload.back() != '\n') {
        payload.push_back('\n');
    }
    ssize_t written = ::write(fd_, payload.data(), payload.size());
    if (written < 0) {
        mark_closed();
        return false;
    }
    return true;
}

}
