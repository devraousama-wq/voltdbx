#pragma once

#include "voltdbx/protocol/command.hpp"
#include "voltdbx/storage/counter_store.hpp"

#include <string>

namespace voltdbx {

std::string handle_incr(const ParsedCommand& cmd, CounterStore& counters);
std::string handle_decr(const ParsedCommand& cmd, CounterStore& counters);
std::string handle_incrby(const ParsedCommand& cmd, CounterStore& counters);

}
