#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/monitor/metrics.hpp"
#include "voltdbx/pubsub/broker.hpp"
#include "voltdbx/storage/batch_ops.hpp"
#include "voltdbx/storage.hpp"

#include <functional>
#include <string>
#include <unordered_map>

namespace voltdbx {

class CommandDispatcher {
public:
    using Handler = std::function<std::string(const ParsedCommand&)>;

    CommandDispatcher(StorageEngine& storage, pubsub::PubSubBroker& broker,
                        monitor::MetricsCollector& metrics);

    std::string dispatch(const ParsedCommand& cmd);

private:
    void register_builtin_handlers();

    StorageEngine& storage_;
    BatchOperations batch_;
    pubsub::PubSubBroker& broker_;
    monitor::MetricsCollector& metrics_;
    std::unordered_map<CommandType, Handler> handlers_;
};

}
