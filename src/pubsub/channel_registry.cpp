#include "voltdbx/pubsub/channel_registry.hpp"

#include <algorithm>

namespace voltdbx::pubsub {

void ChannelRegistry::subscribe(const std::string& channel, SubscriberId subscriber) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& list = channel_map_[channel];
    if (std::find(list.begin(), list.end(), subscriber) == list.end()) {
        list.push_back(subscriber);
    }
    auto& channels = subscriber_map_[subscriber];
    if (std::find(channels.begin(), channels.end(), channel) == channels.end()) {
        channels.push_back(channel);
    }
}

void ChannelRegistry::unsubscribe(const std::string& channel, SubscriberId subscriber) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = channel_map_.find(channel);
    if (it != channel_map_.end()) {
        auto& list = it->second;
        list.erase(std::remove(list.begin(), list.end(), subscriber), list.end());
    }
    auto sub_it = subscriber_map_.find(subscriber);
    if (sub_it != subscriber_map_.end()) {
        auto& channels = sub_it->second;
        channels.erase(std::remove(channels.begin(), channels.end(), channel), channels.end());
    }
}

void ChannelRegistry::unsubscribe_all(SubscriberId subscriber) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto sub_it = subscriber_map_.find(subscriber);
    if (sub_it == subscriber_map_.end()) {
        return;
    }
    for (const auto& channel : sub_it->second) {
        auto it = channel_map_.find(channel);
        if (it != channel_map_.end()) {
            auto& list = it->second;
            list.erase(std::remove(list.begin(), list.end(), subscriber), list.end());
        }
    }
    subscriber_map_.erase(sub_it);
}

std::vector<SubscriberId> ChannelRegistry::subscribers(const std::string& channel) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = channel_map_.find(channel);
    if (it == channel_map_.end()) {
        return {};
    }
    return it->second;
}

std::vector<std::string> ChannelRegistry::channels_for(SubscriberId subscriber) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = subscriber_map_.find(subscriber);
    if (it == subscriber_map_.end()) {
        return {};
    }
    return it->second;
}

}
