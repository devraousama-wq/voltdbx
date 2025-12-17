#include "voltdbx/util/logger.hpp"

#include <cstdlib>
#include <cstring>

namespace voltdbx::util {

namespace {

LogLevel g_level = LogLevel::Info;

LogLevel parse_level(const char* s) {
    if (!s) {
        return LogLevel::Info;
    }
    if (std::strcmp(s, "debug") == 0) {
        return LogLevel::Debug;
    }
    if (std::strcmp(s, "warn") == 0) {
        return LogLevel::Warn;
    }
    if (std::strcmp(s, "error") == 0) {
        return LogLevel::Error;
    }
    return LogLevel::Info;
}

void emit(const char* tag, std::string_view msg) {
    std::cerr << "[" << tag << "] " << msg << "\n";
}

}

void set_log_level(LogLevel level) {
    g_level = level;
}

LogLevel current_log_level() {
    return g_level;
}

void log_info(std::string_view msg) {
    if (static_cast<int>(g_level) <= static_cast<int>(LogLevel::Info)) {
        emit("info", msg);
    }
}

void log_warn(std::string_view msg) {
    if (static_cast<int>(g_level) <= static_cast<int>(LogLevel::Warn)) {
        emit("warn", msg);
    }
}

void log_error(std::string_view msg) {
    emit("error", msg);
}

}
