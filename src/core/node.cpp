#include "node.h"
#include <iostream>
#include <algorithm>

DistributedNode::DistributedNode(const std::string& id, const std::string& addr)
    : node_id(id), address(addr)
{
    hash_ring.addNode(node_id);
}

void DistributedNode::addPeerNode(const std::string& peer_node, const std::string& replica_address) {
    peer_nodes.push_back(peer_node);
    hash_ring.addNode(peer_node);
    network_managers[peer_node] = std::make_unique<NetworkManager>(replica_address);
}

void DistributedNode::put(const std::string& key, const std::string& value) {
    std::string target_node = hash_ring.getNode(key);
    std::cout << "PUT: Key '" << key << "' hashes to node: " << target_node << std::endl;
    if (target_node == node_id) {
        local_store.put(key, value);
    } else {
        auto it = network_managers.find(target_node);
        if (it != network_managers.end()) {
            if (!it->second->put(key, value)) {
                std::cerr << "Failed to put key-value to node: " << target_node << std::endl;
            }
        } else {
            std::cerr << "No network manager for node: " << target_node << std::endl;
        }
    }
}

std::optional<std::string> DistributedNode::get(const std::string& key) {
    std::string target_node = hash_ring.getNode(key);
    std::cout << "GET: Key '" << key << "' hashes to node: " << target_node << std::endl;
    if (target_node == node_id) {
        return local_store.get(key);
    } else {
        auto it = network_managers.find(target_node);
        if (it != network_managers.end()) {
            return it->second->get(key);
        }
        std::cerr << "No network manager for node: " << target_node << std::endl;
        return std::nullopt;
    }
}

void DistributedNode::removeNode(const std::string& failed_node_id) {
    if (failed_node_id == node_id) {
        std::cerr << "Warning: Attempt to remove self node " << failed_node_id << " ignored." << std::endl;
        return;
    }

    hash_ring.removeNode(failed_node_id);
    
    auto it = network_managers.find(failed_node_id);
    if (it != network_managers.end()) {
        network_managers.erase(it);
    }
    
    peer_nodes.erase(std::remove(peer_nodes.begin(), peer_nodes.end(), failed_node_id),peer_nodes.end());
    
    std::cout << "Node " << failed_node_id << " removed from the system." << std::endl;
}
