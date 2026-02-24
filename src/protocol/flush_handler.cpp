#include "voltdbx/protocol/flush_handler.hpp"
#include "voltdbx/protocol/response.hpp"

namespace voltdbx {

std::string handle_flushdb(StorageEngine& storage) {
    storage.clear_all();
    return ok_response();
}

std::string handle_dbsize(const StorageEngine& storage) {
    return integer_response(static_cast<long long>(storage.entry_count()));
}

}
