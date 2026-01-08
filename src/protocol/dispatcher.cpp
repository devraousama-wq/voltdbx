#include "voltdbx/protocol/dispatcher.hpp"
#include "voltdbx/protocol/response.hpp"

namespace voltdbx {

CommandDispatcher::CommandDispatcher(StorageEngine& storage) : storage_(storage) {
    register_builtin_handlers();
}

void CommandDispatcher::register_builtin_handlers() {
    handlers_[CommandType::Ping] = [](const ParsedCommand&) {
        return "+PONG";
    };
    handlers_[CommandType::Info] = [](const ParsedCommand&) {
        return ok_response() + " voltdbx/0.1";
    };
    handlers_[CommandType::Get] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() != 1) {
            return err_response("wrong number of arguments for GET");
        }
        auto value = storage_.get(cmd.args[0]);
        if (!value) {
            return null_bulk_response();
        }
        return bulk_string_response(*value);
    };
    handlers_[CommandType::Delete] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() != 1) {
            return err_response("wrong number of arguments for DELETE");
        }
        const bool removed = storage_.remove(cmd.args[0]);
        return integer_response(removed ? 1 : 0);
    };
    handlers_[CommandType::Exists] = [this](const ParsedCommand& cmd) {
        if (cmd.args.empty()) {
            return err_response("wrong number of arguments for EXISTS");
        }
        long long count = 0;
        for (const auto& key : cmd.args) {
            if (storage_.exists(key)) {
                ++count;
            }
        }
        return integer_response(count);
    };
    handlers_[CommandType::Expire] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() != 2) {
            return err_response("wrong number of arguments for EXPIRE");
        }
        const long long seconds = std::stoll(cmd.args[1]);
        if (!storage_.set_ttl(cmd.args[0], std::chrono::seconds(seconds))) {
            return integer_response(0);
        }
        return integer_response(1);
    };
    handlers_[CommandType::Set] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() < 2) {
            return err_response("wrong number of arguments for SET");
        }
        std::string value = cmd.args[1];
        for (std::size_t i = 2; i < cmd.args.size(); ++i) {
            value.push_back(' ');
            value += cmd.args[i];
        }
        storage_.set(cmd.args[0], value);
        return ok_response();
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
