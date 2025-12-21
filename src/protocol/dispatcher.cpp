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
    handlers_[CommandType::Get] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() != 1) {
            return std::string("-ERR wrong number of arguments for GET");
        }
        auto value = storage_.get(cmd.args[0]);
        if (!value) {
            return std::string("$-1");
        }
        return std::string("$") + std::to_string(value->size()) + "\r\n" + *value;
    };
    handlers_[CommandType::Set] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() < 2) {
            return std::string("-ERR wrong number of arguments for SET");
        }
        std::string value = cmd.args[1];
        for (std::size_t i = 2; i < cmd.args.size(); ++i) {
            value.push_back(' ');
            value += cmd.args[i];
        }
        storage_.set(cmd.args[0], value);
        return std::string("+OK");
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
