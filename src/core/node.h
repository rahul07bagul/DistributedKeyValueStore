#pragma once
#include "storage/key_value_store.h"
#include "core/consistent_hash.h"
#include "network/network_manager.h"
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <optional>

class HeartbeatManager;

class DistributedNode {
public:
    DistributedNode(const std::string& id, const std::string& addr, HeartbeatManager& heartbeatmgr);

    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);

    void addReplicaNode(const std::string& replica_node);
    
    ConsistentHashRing& getHashRing() { return hash_ring; }

    std::string getNodeId() const { return node_id; }
    std::string getAddress() const { return address; }

    std::map<std::string, std::unique_ptr<NetworkManager>>& getPeerConnections() {
        return network_managers;
    }

private:
    static constexpr int REPLICATION_FACTOR = 3;

    std::string node_id;
    std::string address;
    HeartbeatManager& heartbeat_mgr;
    KeyValueStore local_store;
    ConsistentHashRing hash_ring;
    std::vector<std::string> replica_nodes;
    std::map<std::string, std::unique_ptr<NetworkManager>> network_managers;
};