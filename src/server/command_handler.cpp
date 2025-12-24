#include "voltdbx/server/command_handler.hpp"

#include "voltdbx/protocol.hpp"

namespace voltdbx {

CommandHandler::CommandHandler(StorageEngine& storage, CommandDispatcher& dispatcher)
    : storage_(storage), dispatcher_(dispatcher) {}

void CommandHandler::handle_session(net::ClientSession& session) {
    while (session.alive()) {
        std::string line = session.read_line();
        if (!session.alive()) {
            break;
        }
        if (line.empty()) {
            continue;
        }
        auto parsed = parse_line(line);
        if (!parsed) {
            session.write_line("-ERR empty command");
            continue;
        }
        std::string response = dispatcher_.dispatch(*parsed);
        session.write_line(response);
    }
    session.mark_closed();
}

}
