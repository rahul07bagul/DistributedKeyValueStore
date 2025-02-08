#include "node_initializer.h"
#include <algorithm>

std::unique_ptr<NodeContext> NodeInitializer::initialize(const ParsedInput& input) {
    auto context = std::make_unique<NodeContext>();
    
    context->node = std::make_unique<DistributedNode>(input.node_id, input.address);
    context->heartbeat_mgr = std::make_unique<HeartbeatManager>();
    context->cmd_handler = std::make_unique<CommandHandler>(*context->node, 
                                                          input.node_id, 
                                                          input.address, 
                                                          input.peers);

    registerHeartbeatCallback(*context);
    setupPeers(*context, input);
    context->heartbeat_mgr->start();

    return context;
}

void NodeInitializer::registerHeartbeatCallback(NodeContext& context) {
    context.heartbeat_mgr->set_failure_callback(
        [node = context.node.get()](const std::string& failed_node_id) {
            node->removeNode(failed_node_id);
        }
    );
}

void NodeInitializer::setupPeers(NodeContext& context, const ParsedInput& input) {
    std::vector<std::string> all_nodes;
    all_nodes.push_back(input.node_id);
    for (const auto& [peer_id, _] : input.peers) {
        all_nodes.push_back(peer_id);
    }
    std::sort(all_nodes.begin(), all_nodes.end());

    setupHeartbeatConnections(context, input);
}

void NodeInitializer::setupHeartbeatConnections(NodeContext& context, const ParsedInput& input) {
    for (const auto& [peer_id, peer_addr] : input.peers) {
        if (peer_id != input.node_id) {
            context.node->addPeerNode(peer_id, peer_addr);
            context.heartbeat_mgr->register_node(peer_id);
            context.heartbeat_mgr->add_peer_connection(
                peer_id, 
                std::make_unique<NetworkManager>(peer_addr)
            );
        }
    }
}
