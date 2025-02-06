#include "node.h"
#include <iostream>
#include <algorithm>

DistributedNode::DistributedNode(const std::string& id, const std::string& addr)
    : node_id(id), address(addr)
{
    hash_ring.addNode(node_id);
}

void DistributedNode::put(const std::string& key, const std::string& value) {
    std::string primary = hash_ring.getNode(key);
    if (primary == node_id) {
        local_store.put(key, value);
        auto successors = hash_ring.getSuccessors(node_id, REPLICATION_FACTOR - 1);
        for (const auto& replicaNode : successors) {
            auto it = network_managers.find(replicaNode);
            if (it != network_managers.end()) {
                bool ok = it->second->put(key, value);
                if (!ok) {
                    std::cerr << "[WARN] Replication to " << replicaNode
                              << " failed for key=" << key << std::endl;
                }
            }
        }
    } else {
        auto it = network_managers.find(primary);
        if (it != network_managers.end()) {
            bool ok = it->second->put(key, value);
            if (!ok) {
                std::cerr << "Failed to forward put to primary node: "
                          << primary << std::endl;
            }
        } else {
            std::cerr << "No network manager for primary node: "
                      << primary << std::endl;
        }
    }
}

std::optional<std::string> DistributedNode::get(const std::string& key) {
    std::string primary = hash_ring.getNode(key);
    if (primary == node_id) {
        return local_store.get(key);
    }
    auto successors = hash_ring.getSuccessors(primary, REPLICATION_FACTOR - 1);
    bool isReplica = (std::find(successors.begin(), successors.end(), node_id) != successors.end());
    if (isReplica) {
        auto val = local_store.get(key);
        if (val.has_value()) {
            return val;
        }
    }
    auto itPrimary = network_managers.find(primary);
    if (itPrimary != network_managers.end()) {
        auto val = itPrimary->second->get(key);
        if (val.has_value()) {
            return val;
        }
    }
    for (const auto& replicaNode : successors) {
        auto itReplica = network_managers.find(replicaNode);
        if (itReplica != network_managers.end()) {
            auto val = itReplica->second->get(key);
            if (val.has_value()) {
                return val;
            }
        }
    }
    return std::nullopt;
}

// void DistributedNode::put(const std::string& key, const std::string& value) {
//     std::string target_node = hash_ring.getNode(key);
//     if (target_node == node_id) {
//         local_store.put(key, value);
//     } else {
//         auto it = network_managers.find(target_node);
//         if (it != network_managers.end()) {
//             if (!it->second->put(key, value)) {
//                 std::cerr << "Failed to put key-value to node: " << target_node << std::endl;
//             }
//         } else {
//             std::cerr << "No network manager for node: " << target_node << std::endl;
//         }
//     }
// }

// void DistributedNode::put(const std::string& key, const std::string& value) {
//     // 1) Write to our local store
//     local_store.put(key, value);

//     // 2) Forward to ALL known peers
//     //    (We’ll ignore consistent hashing, so remove or comment out your existing lines
//     //     that compute target_node with the ring.)

//     for (const auto& peer : replica_nodes) { 
//         // 'replica_nodes' is your vector<string> of peer addresses
//         auto it = network_managers.find(peer);
//         if (it != network_managers.end()) {
//             bool success = it->second->put(key, value);
//             if (!success) {
//                 std::cerr << "[WARN] Replication to " << peer << " failed for key=" << key << std::endl;
//             }
//         }
//     }
// }

// std::optional<std::string> DistributedNode::get(const std::string& key) {
//     std::string target_node = hash_ring.getNode(key);
//     if (target_node == node_id) {
//         return local_store.get(key);
//     } else {
//         auto it = network_managers.find(target_node);
//         if (it != network_managers.end()) {
//             return it->second->get(key);
//         }
//         std::cerr << "No network manager for node: " << target_node << std::endl;
//         return std::nullopt;
//     }
// }

// std::optional<std::string> DistributedNode::get(const std::string& key) {
//     // 1) Check local
//     auto val = local_store.get(key);
//     if (val.has_value()) {
//         return val;  // found locally
//     }

//     // 2) If not found, ask each peer in turn
//     for (const auto& peer : replica_nodes) {
//         auto it = network_managers.find(peer);
//         if (it != network_managers.end()) {
//             auto peer_val = it->second->get(key);
//             if (peer_val.has_value()) {
//                 return peer_val;  // found on some peer
//             }
//         }
//     }

//     // 3) Not found anywhere
//     return std::nullopt;
// }

void DistributedNode::addReplicaNode(const std::string& replica_node) {
    replica_nodes.push_back(replica_node);
    hash_ring.addNode(replica_node);
    network_managers[replica_node] = std::make_unique<NetworkManager>(replica_node);
}