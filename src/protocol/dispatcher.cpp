#include "voltdbx/protocol/dispatcher.hpp"

namespace voltdbx {

CommandDispatcher::CommandDispatcher(StorageEngine& storage) : storage_(storage) {
    register_builtin_handlers();
}

void CommandDispatcher::register_builtin_handlers() {
    handlers_[CommandType::Ping] = [](const ParsedCommand&) {
        return "+PONG";
    };
    handlers_[CommandType::Info] = [](const ParsedCommand&) {
        return "+OK voltdbx/0.1";
    };
}

std::string CommandDispatcher::dispatch(const ParsedCommand& cmd) {
    auto it = handlers_.find(cmd.type);
    if (it == handlers_.end()) {
        return "-ERR unknown command";
    }
    return it->second(cmd);
}

}
