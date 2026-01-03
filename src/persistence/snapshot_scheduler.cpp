#include "voltdbx/persistence/snapshot_scheduler.hpp"

#include "voltdbx/util/logger.hpp"

namespace voltdbx::persistence {

SnapshotScheduler::SnapshotScheduler(StorageEngine& storage, SnapshotWriter& writer,
                                     std::uint32_t interval_sec)
    : storage_(storage), writer_(writer), interval_sec_(interval_sec) {}

void SnapshotScheduler::start() {
    if (running_.exchange(true)) {
        return;
    }
    worker_ = std::thread([this]() { loop(); });
}

void SnapshotScheduler::stop() {
    if (!running_.exchange(false)) {
        return;
    }
    if (worker_.joinable()) {
        worker_.join();
    }
}

bool SnapshotScheduler::running() const {
    return running_.load();
}

void SnapshotScheduler::loop() {
    while (running_.load()) {
        if (writer_.write(storage_)) {
            util::log_info("snapshot written to " + writer_.latest_path().string());
        } else {
            util::log_warn("snapshot write failed");
        }
        for (std::uint32_t i = 0; i < interval_sec_ && running_.load(); ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

}
