#include "core/command_handler.h"

CommandHandler::CommandHandler(DistributedNode& node, 
                             const std::string& node_id,
                             const std::string& address,
                             const std::unordered_map<std::string, std::string>& peers)
    : node(node), node_id(node_id), address(address), peers(peers) {}

bool CommandHandler::handleCommand(const std::string& command) {
    if (command == "quit" || command == "exit")
        return false;

    std::istringstream iss(command);
    std::string op;
    iss >> op;

    if (op == "put") {
        handlePut(iss);
    } else if (op == "get") {
        handleGet(iss);
    } else if (op == "status") {
        handleStatus();
    } else if (op == "help") {
        printHelp();
    } else {
        std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
    }

    return true;
}

void CommandHandler::handlePut(std::istringstream& iss) {
    std::string key, value;
    if (iss >> key >> value) {
        node.put(key, value);
        std::cout << "Put: " << key << " -> " << value << std::endl;
    } else {
        std::cout << "Usage: put <key> <value>" << std::endl;
    }
}

void CommandHandler::handleGet(std::istringstream& iss) {
    std::string key;
    if (iss >> key) {
        auto val = node.get(key);
        if (val)
            std::cout << "Get: " << key << " -> " << *val << std::endl;
        else
            std::cout << "Key not found: " << key << std::endl;
    } else {
        std::cout << "Usage: get <key>" << std::endl;
    }
}

void CommandHandler::handleStatus() {
    std::cout << "Node ID: " << node_id 
              << "\nAddress: " << address 
              << "\nPeers: ";
    for (const auto& [peer_id, peer_addr] : peers)
        std::cout << peer_id << "(" << peer_addr << ") ";
    std::cout << std::endl;
}

void CommandHandler::printHelp() const {
    std::cout << "Available commands:\n"
              << "  put <key> <value>  - Store a key-value pair\n"
              << "  get <key>         - Retrieve a value by key\n"
              << "  status            - Show node status and peers\n"
              << "  help              - Show this help message\n"
              << "  quit/exit         - Exit the program" << std::endl;
}
