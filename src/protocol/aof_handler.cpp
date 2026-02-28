#include "voltdbx/protocol/aof_handler.hpp"
#include "voltdbx/protocol/response.hpp"

#include <sstream>

namespace voltdbx {

std::string handle_bgrewriteaof(persistence::AofLog& aof, StorageEngine& storage) {
    const std::size_t before = aof.bytes_on_disk();
    if (!aof.rewrite_from(storage)) {
        return err_response("aof rewrite failed");
    }
    const std::size_t after = aof.bytes_on_disk();
    std::ostringstream out;
    out << "+OK rewritten bytes_before=" << before << " bytes_after=" << after;
    return out.str();
}

}
