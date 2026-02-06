#pragma once

#include "voltdbx/net/client_session.hpp"
#include "voltdbx/protocol/dispatcher.hpp"
#include "voltdbx/storage.hpp"

namespace voltdbx {

class CommandHandler {
public:
    CommandHandler(StorageEngine& storage, CommandDispatcher& dispatcher);

    void handle_session(net::ClientSession& session);

private:
    StorageEngine& storage_;
    CommandDispatcher& dispatcher_;
};

}
