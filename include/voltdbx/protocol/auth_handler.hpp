#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/security/auth_gate.hpp"

#include <string>

namespace voltdbx {

std::string handle_auth(const ParsedCommand& cmd, security::AuthGate& gate);

}
