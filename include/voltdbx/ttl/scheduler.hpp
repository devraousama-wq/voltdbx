#pragma once

#include "voltdbx/storage.hpp"
#include "voltdbx/ttl/expiration.hpp"

#include <atomic>
#include <chrono>
#include <thread>

namespace voltdbx::ttl {

class ExpirationScheduler {
public:
    ExpirationScheduler(StorageEngine& storage, ExpirationService& expiration,
                        std::chrono::milliseconds interval);

    void start();
    void stop();

private:
    void loop();

    StorageEngine& storage_;
    ExpirationService& expiration_;
    std::chrono::milliseconds interval_;
    std::atomic<bool> running_{false};
    std::thread worker_;
};

}
