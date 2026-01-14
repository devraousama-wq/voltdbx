#include "voltdbx/pubsub/session_mailbox.hpp"

namespace voltdbx::pubsub {

void SessionMailbox::bind(SubscriberId) {}

void SessionMailbox::enqueue(SubscriberId id, std::string message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queues_[id].push(std::move(message));
}

bool SessionMailbox::try_dequeue(SubscriberId id, std::string& out) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = queues_.find(id);
    if (it == queues_.end() || it->second.empty()) {
        return false;
    }
    out = std::move(it->second.front());
    it->second.pop();
    return true;
}

void SessionMailbox::clear(SubscriberId id) {
    std::lock_guard<std::mutex> lock(mutex_);
    queues_.erase(id);
}

void wire_broker_delivery(PubSubBroker& broker, SessionMailbox& mailbox) {
    broker.set_delivery([&mailbox](SubscriberId id, const std::string& message) {
        mailbox.enqueue(id, message);
    });
}

}
