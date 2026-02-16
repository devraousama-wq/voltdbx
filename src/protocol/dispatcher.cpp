#include "voltdbx/protocol/dispatcher.hpp"
#include "voltdbx/protocol/auth_handler.hpp"
#include "voltdbx/protocol/batch_handler.hpp"
#include "voltdbx/protocol/counter_handler.hpp"
#include "voltdbx/protocol/scan_handler.hpp"
#include "voltdbx/protocol/response.hpp"

#include <chrono>

namespace voltdbx {

CommandDispatcher::CommandDispatcher(StorageEngine& storage, pubsub::PubSubBroker& broker,
                                     monitor::MetricsCollector& metrics, security::AuthGate& auth)
    : storage_(storage), batch_(storage), counters_(storage), broker_(broker), metrics_(metrics),
      auth_(auth) {
    register_builtin_handlers();
}

void CommandDispatcher::register_builtin_handlers() {
    handlers_[CommandType::Ping] = [](const ParsedCommand&) {
        return "+PONG";
    };
    handlers_[CommandType::Info] = [this](const ParsedCommand&) {
        metrics_.set_keys_stored(storage_.entry_count());
        return metrics_.render_info();
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
    handlers_[CommandType::Subscribe] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() != 1) {
            return err_response("wrong number of arguments for SUBSCRIBE");
        }
        broker_.register_subscriber(1, cmd.args[0]);
        return ok_response() + " subscribed " + cmd.args[0];
    };
    handlers_[CommandType::Publish] = [this](const ParsedCommand& cmd) {
        if (cmd.args.size() < 2) {
            return err_response("wrong number of arguments for PUBLISH");
        }
        std::string message = cmd.args[1];
        for (std::size_t i = 2; i < cmd.args.size(); ++i) {
            message.push_back(' ');
            message += cmd.args[i];
        }
        const std::size_t delivered = broker_.publish(cmd.args[0], message);
        return integer_response(static_cast<long long>(delivered));
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
    handlers_[CommandType::Mget] = [this](const ParsedCommand& cmd) {
        return handle_mget(cmd, batch_);
    };
    handlers_[CommandType::Mset] = [this](const ParsedCommand& cmd) {
        return handle_mset(cmd, batch_);
    };
    handlers_[CommandType::Incr] = [this](const ParsedCommand& cmd) {
        return handle_incr(cmd, counters_);
    };
    handlers_[CommandType::Decr] = [this](const ParsedCommand& cmd) {
        return handle_decr(cmd, counters_);
    };
    handlers_[CommandType::Incrby] = [this](const ParsedCommand& cmd) {
        return handle_incrby(cmd, counters_);
    };
    handlers_[CommandType::Scan] = [this](const ParsedCommand& cmd) {
        return handle_scan(cmd, storage_);
    };
    handlers_[CommandType::Auth] = [this](const ParsedCommand& cmd) {
        return handle_auth(cmd, auth_);
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
    metrics_.record_command();
    return it->second(cmd);
}

}
