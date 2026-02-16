#include "voltdbx/security/auth_gate.hpp"

namespace voltdbx::security {

AuthGate::AuthGate(std::string password) : password_(std::move(password)) {}

bool AuthGate::enabled() const {
    return !password_.empty();
}

bool AuthGate::authenticate(std::string_view password) const {
    if (!enabled()) {
        return true;
    }
    return password_ == password;
}

void AuthGate::set_password(std::string password) {
    password_ = std::move(password);
}

std::string AuthGate::masked_password_hint() const {
    if (password_.empty()) {
        return "";
    }
    return std::string(password_.size(), '*');
}

}
