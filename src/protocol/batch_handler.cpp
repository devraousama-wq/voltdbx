#include "voltdbx/protocol/batch_handler.hpp"
#include "voltdbx/protocol/response.hpp"

#include <sstream>

namespace voltdbx {

std::string handle_mget(const ParsedCommand& cmd, BatchOperations& batch) {
    if (cmd.args.empty()) {
        return err_response("wrong number of arguments for MGET");
    }
    const auto values = batch.mget(cmd.args);
    std::ostringstream out;
    out << "*" << values.size() << "\r\n";
    for (const auto& value : values) {
        if (!value) {
            out << null_bulk_response() << "\r\n";
        } else {
            out << bulk_string_response(*value) << "\r\n";
        }
    }
    return out.str();
}

std::string handle_mset(const ParsedCommand& cmd, BatchOperations& batch) {
    if (cmd.args.size() < 2 || cmd.args.size() % 2 != 0) {
        return err_response("wrong number of arguments for MSET");
    }
    std::vector<BatchSetPair> pairs;
    for (std::size_t i = 0; i + 1 < cmd.args.size(); i += 2) {
        pairs.push_back(BatchSetPair{cmd.args[i], cmd.args[i + 1]});
    }
    batch.mset(pairs);
    return ok_response();
}

}
