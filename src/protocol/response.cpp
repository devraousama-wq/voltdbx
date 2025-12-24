#include "voltdbx/protocol/response.hpp"

namespace voltdbx {

std::string ok_response() {
    return "+OK";
}

std::string err_response(std::string_view message) {
    return std::string("-ERR ") + std::string(message);
}

std::string integer_response(long long value) {
    return std::string(":") + std::to_string(value);
}

std::string bulk_string_response(const std::string& value) {
    return std::string("$") + std::to_string(value.size()) + "\r\n" + value;
}

std::string null_bulk_response() {
    return "$-1";
}

}
