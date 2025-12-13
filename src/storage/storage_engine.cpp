#include "voltdbx/storage.hpp"

namespace voltdbx {

bool StorageEngine::set(std::string_view, std::string_view) {
    return false;
}

std::optional<std::string> StorageEngine::get(std::string_view) const {
    return std::nullopt;
}

bool StorageEngine::remove(std::string_view) {
    return false;
}

bool StorageEngine::exists(std::string_view) const {
    return false;
}

}
