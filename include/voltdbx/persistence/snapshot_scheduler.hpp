#pragma once

#include "voltdbx/persistence/snapshot.hpp"
#include "voltdbx/storage.hpp"

#include <atomic>
#include <chrono>
#include <thread>

namespace voltdbx::persistence {

class SnapshotScheduler {
public:
    SnapshotScheduler(StorageEngine& storage, SnapshotWriter& writer, std::uint32_t interval_sec);

    void start();
    void stop();
    bool running() const;

private:
    void loop();

    StorageEngine& storage_;
    SnapshotWriter& writer_;
    std::uint32_t interval_sec_;
    std::atomic<bool> running_{false};
    std::thread worker_;
};

}
