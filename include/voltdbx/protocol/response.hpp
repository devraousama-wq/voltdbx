#pragma once

#include <string>
#include <string_view>

namespace voltdbx {

std::string ok_response();
std::string err_response(std::string_view message);
std::string integer_response(long long value);
std::string bulk_string_response(const std::string& value);
std::string null_bulk_response();

}
