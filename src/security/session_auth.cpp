#include "voltdbx/security/session_auth.hpp"

namespace voltdbx::security {

SessionAuth::SessionAuth(security::AuthGate& gate, bool required)
    : gate_(gate), required_(required), authenticated_(!required || !gate.enabled()) {}

bool SessionAuth::ensure_authenticated(net::ClientSession& session, std::string_view password) {
    if (!required_ || !gate_.enabled() || authenticated_) {
        return true;
    }
    if (gate_.authenticate(password)) {
        authenticated_ = true;
        return true;
    }
    session.write_line("-ERR invalid password");
    return false;
}

void SessionAuth::reset() {
    authenticated_ = !required_ || !gate_.enabled();
}

}
