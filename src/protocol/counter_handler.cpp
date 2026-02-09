#include "voltdbx/protocol/counter_handler.hpp"
#include "voltdbx/protocol/response.hpp"

namespace voltdbx {

std::string handle_incr(const ParsedCommand& cmd, CounterStore& counters) {
    if (cmd.args.size() != 1) {
        return err_response("wrong number of arguments for INCR");
    }
    return integer_response(counters.increment(cmd.args[0], 1));
}

std::string handle_decr(const ParsedCommand& cmd, CounterStore& counters) {
    if (cmd.args.size() != 1) {
        return err_response("wrong number of arguments for DECR");
    }
    return integer_response(counters.decrement(cmd.args[0], 1));
}

std::string handle_incrby(const ParsedCommand& cmd, CounterStore& counters) {
    if (cmd.args.size() != 2) {
        return err_response("wrong number of arguments for INCRBY");
    }
    const long long delta = std::stoll(cmd.args[1]);
    return integer_response(counters.increment(cmd.args[0], delta));
}

}
