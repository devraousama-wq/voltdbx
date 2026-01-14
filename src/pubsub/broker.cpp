#include "voltdbx/pubsub/broker.hpp"

namespace voltdbx::pubsub {

PubSubBroker::PubSubBroker(ChannelRegistry& registry) : registry_(registry) {}

void PubSubBroker::set_delivery(DeliveryFn delivery) {
    delivery_ = std::move(delivery);
}

std::size_t PubSubBroker::publish(const std::string& channel, const std::string& message) {
    const auto targets = registry_.subscribers(channel);
    for (SubscriberId id : targets) {
        if (delivery_) {
            delivery_(id, message);
        }
    }
    return targets.size();
}

void PubSubBroker::register_subscriber(SubscriberId id, const std::string& channel) {
    registry_.subscribe(channel, id);
}

void PubSubBroker::remove_subscriber(SubscriberId id) {
    registry_.unsubscribe_all(id);
}

}
