#include "voltdbx/persistence/aof_log.hpp"

#include <sstream>

namespace voltdbx::persistence {

AofLog::AofLog(std::filesystem::path data_dir)
    : log_path_(std::move(data_dir) / "append.aof") {
    std::filesystem::create_directories(log_path_.parent_path());
    stream_.open(log_path_, std::ios::app);
}

void AofLog::write_line(const std::string& line) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!stream_.is_open()) {
        stream_.open(log_path_, std::ios::app);
    }
    stream_ << line << "\n";
    stream_.flush();
}

void AofLog::append_set(std::string_view key, std::string_view value) {
    std::ostringstream line;
    line << "SET " << key << " " << value;
    write_line(line.str());
}

void AofLog::append_del(std::string_view key) {
    std::ostringstream line;
    line << "DEL " << key;
    write_line(line.str());
}

bool AofLog::replay(StorageEngine& storage) {
    std::ifstream in(log_path_);
    if (!in) {
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("SET ", 0) == 0) {
            const std::size_t space = line.find(' ', 4);
            if (space == std::string::npos) {
                continue;
            }
            const std::string key = line.substr(4, space - 4);
            const std::string value = line.substr(space + 1);
            storage.set(key, value);
        } else if (line.rfind("DEL ", 0) == 0) {
            storage.remove(line.substr(4));
        }
    }
    return true;
}

void AofLog::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stream_.is_open()) {
        stream_.flush();
    }
}

bool AofLog::rewrite_from(const StorageEngine& storage) {
    const auto entries = storage.dump_entries();
    const std::filesystem::path temp_path = log_path_.string() + ".rewrite";
    {
        std::ofstream out(temp_path, std::ios::trunc);
        if (!out) {
            return false;
        }
        out << "AOFREWRITE_V1\n";
        for (const auto& entry : entries) {
            out << "SET " << entry.first << " " << entry.second << "\n";
        }
    }
    std::lock_guard<std::mutex> lock(mutex_);
    if (stream_.is_open()) {
        stream_.close();
    }
    std::error_code ec;
    std::filesystem::remove(log_path_, ec);
    std::filesystem::rename(temp_path, log_path_, ec);
    if (ec) {
        return false;
    }
    stream_.open(log_path_, std::ios::app);
    return true;
}

std::size_t AofLog::bytes_on_disk() const {
    std::error_code ec;
    if (!std::filesystem::exists(log_path_, ec)) {
        return 0;
    }
    return static_cast<std::size_t>(std::filesystem::file_size(log_path_, ec));
}

}
