#pragma once

#include "voltdbx/storage.hpp"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

namespace voltdbx::persistence {

class AofLog {
public:
    explicit AofLog(std::filesystem::path data_dir);

    void append_set(std::string_view key, std::string_view value);
    void append_del(std::string_view key);
    bool replay(StorageEngine& storage);
    void flush();

private:
    void write_line(const std::string& line);

    std::filesystem::path log_path_;
    std::ofstream stream_;
    std::mutex mutex_;
};

}
