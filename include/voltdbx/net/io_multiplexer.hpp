#pragma once

#include <cstdint>
#include <functional>
#include <vector>

namespace voltdbx::net {

class IoMultiplexer {
public:
    IoMultiplexer();
    ~IoMultiplexer();

    IoMultiplexer(const IoMultiplexer&) = delete;
    IoMultiplexer& operator=(const IoMultiplexer&) = delete;

    bool add_read(int fd);
    bool remove(int fd);
    int wait(std::vector<int>& ready, int timeout_ms);
    void set_readable_callback(std::function<void(int)> callback);

private:
    int poll_fd_ = -1;
    std::function<void(int)> on_readable_;
};

}
