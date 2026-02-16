#include "voltdbx/protocol/auth_handler.hpp"
#include "voltdbx/protocol/response.hpp"

namespace voltdbx {

std::string handle_auth(const ParsedCommand& cmd, security::AuthGate& gate) {
    if (cmd.args.size() != 1) {
        return err_response("wrong number of arguments for AUTH");
    }
    if (gate.authenticate(cmd.args[0])) {
        return ok_response();
    }
    return err_response("invalid password");
}

}
