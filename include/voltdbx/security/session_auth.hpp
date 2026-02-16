#pragma once

#include "voltdbx/net/client_session.hpp"
#include "voltdbx/security/auth_gate.hpp"

namespace voltdbx::security {

class SessionAuth {
public:
    SessionAuth(security::AuthGate& gate, bool required);

    bool ensure_authenticated(net::ClientSession& session, std::string_view password);
    bool is_authenticated() const { return authenticated_; }
    void reset();

private:
    security::AuthGate& gate_;
    bool required_;
    bool authenticated_ = false;
};

}
