#include "voltdbx/persistence/snapshot.hpp"

#include <fstream>
#include <sstream>

namespace voltdbx::persistence {

SnapshotWriter::SnapshotWriter(std::filesystem::path data_dir)
    : data_dir_(std::move(data_dir)),
      snapshot_path_(data_dir_ / "snapshot.vdb") {
    std::filesystem::create_directories(data_dir_);
}

bool SnapshotWriter::write(const StorageEngine& storage) {
    std::ofstream out(snapshot_path_, std::ios::trunc);
    if (!out) {
        return false;
    }
    const auto pairs = storage.dump_entries();
    out << "VOLTDBX_SNAPSHOT_V1\n";
    out << pairs.size() << "\n";
    for (const auto& pair : pairs) {
        out << pair.first << "\n" << pair.second << "\n";
    }
    return out.good();
}

std::filesystem::path SnapshotWriter::latest_path() const {
    return snapshot_path_;
}

SnapshotLoader::SnapshotLoader(std::filesystem::path data_dir)
    : snapshot_path_(std::move(data_dir) / "snapshot.vdb") {}

bool SnapshotLoader::has_snapshot() const {
    return std::filesystem::exists(snapshot_path_);
}

bool SnapshotLoader::load_into(StorageEngine& storage) {
    if (!has_snapshot()) {
        return false;
    }
    std::ifstream in(snapshot_path_);
    if (!in) {
        return false;
    }
    std::string header;
    std::getline(in, header);
    if (header != "VOLTDBX_SNAPSHOT_V1") {
        return false;
    }
    std::string count_line;
    std::getline(in, count_line);
    std::istringstream count_stream(count_line);
    std::size_t count = 0;
    count_stream >> count;
    std::string key;
    std::string value;
    for (std::size_t i = 0; i < count; ++i) {
        if (!std::getline(in, key) || !std::getline(in, value)) {
            break;
        }
        storage.set(key, value);
    }
    return true;
}

}
