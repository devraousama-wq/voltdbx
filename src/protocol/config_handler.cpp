#include "voltdbx/protocol/config_handler.hpp"
#include "voltdbx/protocol/response.hpp"

namespace voltdbx {

std::string handle_config(const ParsedCommand& cmd, RuntimeConfig& runtime) {
    if (cmd.args.empty()) {
        return bulk_string_response(runtime.dump());
    }
    if (cmd.args[0] == "GET" && cmd.args.size() == 2) {
        const auto value = runtime.get(cmd.args[1]);
        if (!value) {
            return err_response("unknown config key");
        }
        return bulk_string_response(*value);
    }
    if (cmd.args[0] == "SET" && cmd.args.size() == 3) {
        if (!runtime.set(cmd.args[1], cmd.args[2])) {
            return err_response("unsupported config key");
        }
        return ok_response();
    }
    return err_response("unsupported CONFIG subcommand");
}

}
