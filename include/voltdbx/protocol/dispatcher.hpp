#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/monitor/metrics.hpp"
#include "voltdbx/pubsub/broker.hpp"
#include "voltdbx/storage/batch_ops.hpp"
#include "voltdbx/storage/counter_store.hpp"
#include "voltdbx/security/auth_gate.hpp"
#include "voltdbx/storage.hpp"

#include <functional>
#include <string>
#include <unordered_map>

namespace voltdbx {

class CommandDispatcher {
public:
    using Handler = std::function<std::string(const ParsedCommand&)>;

    CommandDispatcher(StorageEngine& storage, pubsub::PubSubBroker& broker,
                        monitor::MetricsCollector& metrics, security::AuthGate& auth);

    std::string dispatch(const ParsedCommand& cmd);

private:
    void register_builtin_handlers();

    StorageEngine& storage_;
    BatchOperations batch_;
    CounterStore counters_;
    pubsub::PubSubBroker& broker_;
    monitor::MetricsCollector& metrics_;
    security::AuthGate& auth_;
    std::unordered_map<CommandType, Handler> handlers_;
};

}
