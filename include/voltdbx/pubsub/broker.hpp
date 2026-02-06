#pragma once

#include "voltdbx/pubsub/channel_registry.hpp"

#include <functional>
#include <string>

namespace voltdbx::pubsub {

using DeliveryFn = std::function<void(SubscriberId, const std::string& message)>;

class PubSubBroker {
public:
    explicit PubSubBroker(ChannelRegistry& registry);

    void set_delivery(DeliveryFn delivery);
    std::size_t publish(const std::string& channel, const std::string& message);
    void register_subscriber(SubscriberId id, const std::string& channel);
    void remove_subscriber(SubscriberId id);

private:
    ChannelRegistry& registry_;
    DeliveryFn delivery_;
};

}
