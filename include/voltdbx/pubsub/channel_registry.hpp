#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace voltdbx::pubsub {

using SubscriberId = std::uint64_t;

class ChannelRegistry {
public:
    void subscribe(const std::string& channel, SubscriberId subscriber);
    void unsubscribe(const std::string& channel, SubscriberId subscriber);
    void unsubscribe_all(SubscriberId subscriber);
    std::vector<SubscriberId> subscribers(const std::string& channel) const;
    std::vector<std::string> channels_for(SubscriberId subscriber) const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::vector<SubscriberId>> channel_map_;
    std::unordered_map<SubscriberId, std::vector<std::string>> subscriber_map_;
};

}
