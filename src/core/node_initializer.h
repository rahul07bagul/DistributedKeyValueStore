#pragma once

#include <memory>
#include <string>
#include "core/node.h"
#include "core/heartbeat_manager.h"
#include "core/command_handler.h"
#include "core/input_parser.h"
#include "network/network_manager.h"

struct NodeContext {
    std::unique_ptr<DistributedNode> node;
    std::unique_ptr<HeartbeatManager> heartbeat_mgr;
    std::unique_ptr<CommandHandler> cmd_handler;
};

class NodeInitializer {
public:
    static std::unique_ptr<NodeContext> initialize(const ParsedInput& input);
    static void setupPeers(NodeContext& context, const ParsedInput& input);

private:
    static void registerHeartbeatCallback(NodeContext& context);
    static void setupHeartbeatConnections(NodeContext& context, const ParsedInput& input);
};
