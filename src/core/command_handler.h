#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "core/node.h"

class CommandHandler {
public:
    explicit CommandHandler(DistributedNode& node, 
                          const std::string& node_id,
                          const std::string& address,
                          const std::unordered_map<std::string, std::string>& peers);

    bool handleCommand(const std::string& command);
    void printHelp() const;

private:
    void handlePut(std::istringstream& iss);
    void handleGet(std::istringstream& iss);
    void handleStatus();

    DistributedNode& node;
    std::string node_id;
    std::string address;
    std::unordered_map<std::string, std::string> peers;
};
