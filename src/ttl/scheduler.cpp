#include "voltdbx/ttl/scheduler.hpp"

#include "voltdbx/util/logger.hpp"

namespace voltdbx::ttl {

ExpirationScheduler::ExpirationScheduler(StorageEngine& storage, ExpirationService& expiration,
                                         std::chrono::milliseconds interval)
    : storage_(storage), expiration_(expiration), interval_(interval) {}

void ExpirationScheduler::start() {
    if (running_.exchange(true)) {
        return;
    }
    worker_ = std::thread([this]() { loop(); });
}

void ExpirationScheduler::stop() {
    if (!running_.exchange(false)) {
        return;
    }
    if (worker_.joinable()) {
        worker_.join();
    }
}

void ExpirationScheduler::loop() {
    while (running_.load()) {
        const std::size_t before = storage_.entry_count();
        storage_.purge_expired();
        const std::size_t after = storage_.entry_count();
        if (before != after) {
            util::log_info("ttl sweep removed " + std::to_string(before - after) + " keys");
        }
        std::this_thread::sleep_for(interval_);
    }
}

}
