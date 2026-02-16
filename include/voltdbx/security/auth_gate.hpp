#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace voltdbx::security {

class AuthGate {
public:
    explicit AuthGate(std::string password);

    bool enabled() const;
    bool authenticate(std::string_view password) const;
    void set_password(std::string password);
    std::string masked_password_hint() const;

private:
    std::string password_;
};

}
