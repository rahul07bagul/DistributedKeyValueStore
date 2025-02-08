#include "input_parser.h"
#include <iostream>
#include <sstream>

InputParser::InputParser(int argc, char* argv[]) : argc(argc), argv(argv) {}

void InputParser::print_usage() {
    std::cout << "Usage: kvstore_node [OPTIONS]\n"
              << "Options:\n"
              << "  --node-id=<id>          Unique identifier for this node\n"
              << "  --address=<addr>        Address to listen on (e.g., 0.0.0.0:50051)\n"
              << "  --peers=<id:addr,...>   Comma-separated list of peer id:address pairs\n";
}

std::vector<std::string> InputParser::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool InputParser::parse_peers(const std::string& peers_str, 
                            std::unordered_map<std::string, std::string>& peers) {
    auto entries = split(peers_str, ',');
    for (const auto& entry : entries) {
        auto pos = entry.find(':');
        if (pos == std::string::npos) {
            std::cerr << "Invalid peer format. Expected id:address, got: " << entry << std::endl;
            return false;
        }
        std::string peer_id = entry.substr(0, pos);
        std::string peer_address = entry.substr(pos + 1);
        peers[peer_id] = peer_address;
    }
    return true;
}

std::optional<ParsedInput> InputParser::parse() {
    ParsedInput input;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help") {
            print_usage();
            return std::nullopt;
        }

        if (arg.find("--node-id=") == 0) {
            input.node_id = arg.substr(10);
        } else if (arg.find("--address=") == 0) {
            input.address = arg.substr(10);
        } else if (arg.find("--peers=") == 0) {
            if (!parse_peers(arg.substr(8), input.peers)) {
                return std::nullopt;
            }
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            print_usage();
            return std::nullopt;
        }
    }

    if (input.node_id.empty() || input.address.empty()) {
        std::cerr << "Error: node-id and address are required\n";
        print_usage();
        return std::nullopt;
    }

    return input;
}
