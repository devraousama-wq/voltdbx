#pragma once

#include "voltdbx/storage.hpp"

#include <filesystem>
#include <string>

namespace voltdbx::persistence {

class SnapshotWriter {
public:
    explicit SnapshotWriter(std::filesystem::path data_dir);

    bool write(const StorageEngine& storage);
    std::filesystem::path latest_path() const;

private:
    std::filesystem::path data_dir_;
    std::filesystem::path snapshot_path_;
};

class SnapshotLoader {
public:
    explicit SnapshotLoader(std::filesystem::path data_dir);

    bool load_into(StorageEngine& storage);
    bool has_snapshot() const;

private:
    std::filesystem::path snapshot_path_;
};

}
