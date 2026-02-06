#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace voltdbx::util {

enum class LogLevel { Debug, Info, Warn, Error };

void set_log_level(LogLevel level);
LogLevel current_log_level();

void log_info(std::string_view msg);
void log_warn(std::string_view msg);
void log_error(std::string_view msg);

}
