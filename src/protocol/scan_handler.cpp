#include "voltdbx/protocol/scan_handler.hpp"
#include "voltdbx/protocol/response.hpp"

#include <sstream>

namespace voltdbx {

ScanCursor decode_scan_cursor(const std::string& token) {
    ScanCursor cursor;
    if (token.empty() || token == "0") {
        return cursor;
    }
    const std::size_t sep = token.find(':');
    if (sep == std::string::npos) {
        cursor.bucket = static_cast<std::uint64_t>(std::stoull(token));
        return cursor;
    }
    cursor.bucket = static_cast<std::uint64_t>(std::stoull(token.substr(0, sep)));
    cursor.chain = static_cast<std::uint64_t>(std::stoull(token.substr(sep + 1)));
    return cursor;
}

std::string encode_scan_cursor(const ScanCursor& cursor) {
    if (cursor.bucket == 0 && cursor.chain == 0) {
        return "0";
    }
    return std::to_string(cursor.bucket) + ":" + std::to_string(cursor.chain);
}

std::string handle_scan(const ParsedCommand& cmd, StorageEngine& storage) {
    if (cmd.args.size() < 1) {
        return err_response("wrong number of arguments for SCAN");
    }
    ScanCursor cursor = decode_scan_cursor(cmd.args[0]);
    std::size_t count = 10;
    std::string pattern = "*";
    for (std::size_t i = 1; i < cmd.args.size(); ++i) {
        if (cmd.args[i] == "COUNT" && i + 1 < cmd.args.size()) {
            count = static_cast<std::size_t>(std::stoull(cmd.args[++i]));
        } else if (cmd.args[i] == "MATCH" && i + 1 < cmd.args.size()) {
            pattern = cmd.args[++i];
        }
    }
    const ScanPage page = storage.scan_keys(cursor, count, pattern);
    const std::string next = page.finished ? "0" : encode_scan_cursor(page.next);
    std::ostringstream out;
    out << "*2\r\n" << bulk_string_response(next) << "\r\n";
    out << "*" << page.keys.size() << "\r\n";
    for (const auto& key : page.keys) {
        out << bulk_string_response(key) << "\r\n";
    }
    return out.str();
}

}
