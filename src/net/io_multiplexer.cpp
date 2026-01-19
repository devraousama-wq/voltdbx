#include "voltdbx/net/io_multiplexer.hpp"

#if defined(__APPLE__)
#include <sys/event.h>
#include <sys/types.h>
#else
#include <sys/epoll.h>
#include <unistd.h>
#endif

namespace voltdbx::net {

IoMultiplexer::IoMultiplexer() {
#if defined(__APPLE__)
    poll_fd_ = kqueue();
#else
    poll_fd_ = epoll_create1(0);
#endif
}

IoMultiplexer::~IoMultiplexer() {
    if (poll_fd_ >= 0) {
#if !defined(__APPLE__)
        close(poll_fd_);
#endif
        poll_fd_ = -1;
    }
}

bool IoMultiplexer::add_read(int fd) {
#if defined(__APPLE__)
    kevent changelist{};
    changelist.ident = static_cast<uintptr_t>(fd);
    changelist.filter = EVFILT_READ;
    changelist.flags = EV_ADD;
    return kevent(poll_fd_, &changelist, 1, nullptr, 0, nullptr) == 0;
#else
    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    return epoll_ctl(poll_fd_, EPOLL_CTL_ADD, fd, &ev) == 0;
#endif
}

bool IoMultiplexer::remove(int fd) {
#if defined(__APPLE__)
    kevent changelist{};
    changelist.ident = static_cast<uintptr_t>(fd);
    changelist.filter = EVFILT_READ;
    changelist.flags = EV_DELETE;
    return kevent(poll_fd_, &changelist, 1, nullptr, 0, nullptr) == 0;
#else
    return epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd, nullptr) == 0;
#endif
}

int IoMultiplexer::wait(std::vector<int>& ready, int timeout_ms) {
    ready.clear();
#if defined(__APPLE__)
    timespec timeout{};
    timespec* timeout_ptr = nullptr;
    if (timeout_ms >= 0) {
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_nsec = (timeout_ms % 1000) * 1000000L;
        timeout_ptr = &timeout;
    }
    std::vector<kevent> events(64);
    const int count = kevent(poll_fd_, nullptr, 0, events.data(),
                             static_cast<int>(events.size()), timeout_ptr);
    if (count <= 0) {
        return count;
    }
    for (int i = 0; i < count; ++i) {
        ready.push_back(static_cast<int>(events[static_cast<std::size_t>(i)].ident));
        if (on_readable_) {
            on_readable_(ready.back());
        }
    }
    return count;
#else
    std::vector<epoll_event> events(64);
    const int count = epoll_wait(poll_fd_, events.data(), static_cast<int>(events.size()), timeout_ms);
    if (count <= 0) {
        return count;
    }
    for (int i = 0; i < count; ++i) {
        ready.push_back(events[static_cast<std::size_t>(i)].data.fd);
        if (on_readable_) {
            on_readable_(ready.back());
        }
    }
    return count;
#endif
}

void IoMultiplexer::set_readable_callback(std::function<void(int)> callback) {
    on_readable_ = std::move(callback);
}

}
