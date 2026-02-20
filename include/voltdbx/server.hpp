#pragma once

#include "voltdbx/config.hpp"
#include "voltdbx/persistence/aof_log.hpp"
#include "voltdbx/protocol/dispatcher.hpp"
#include "voltdbx/pubsub/broker.hpp"
#include "voltdbx/monitor/metrics.hpp"
#include "voltdbx/pubsub/channel_registry.hpp"
#include "voltdbx/config/runtime_config.hpp"
#include "voltdbx/security/auth_gate.hpp"
#include "voltdbx/storage.hpp"

#include <memory>

namespace voltdbx {

class DatabaseServer {
public:
    explicit DatabaseServer(ServerConfig config);
    int run();

private:
    ServerConfig config_;
    StorageEngine storage_;
    persistence::AofLog aof_;
    pubsub::ChannelRegistry channels_;
    pubsub::PubSubBroker broker_;
    monitor::MetricsCollector metrics_;
    security::AuthGate auth_;
    RuntimeConfig runtime_;
    std::unique_ptr<CommandDispatcher> dispatcher_;
};

}
