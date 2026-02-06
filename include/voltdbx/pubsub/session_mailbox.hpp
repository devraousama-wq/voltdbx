#pragma once

#include "voltdbx/pubsub/broker.hpp"

#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

namespace voltdbx::pubsub {

class SessionMailbox {
public:
    void bind(SubscriberId id);
    void enqueue(SubscriberId id, std::string message);
    bool try_dequeue(SubscriberId id, std::string& out);
    void clear(SubscriberId id);

private:
    std::mutex mutex_;
    std::unordered_map<SubscriberId, std::queue<std::string>> queues_;
};

void wire_broker_delivery(PubSubBroker& broker, SessionMailbox& mailbox);

}
