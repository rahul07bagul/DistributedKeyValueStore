#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <functional>
#include <string>
#include "network/network_manager.h"
#include "node.h"

class DistributedNode;

class HeartbeatManager {
private:
    struct NodeStatus {
        std::chrono::steady_clock::time_point last_heartbeat;
        bool is_alive{false};
    };

    DistributedNode* distributed_node = nullptr;
    std::unordered_map<std::string, NodeStatus> node_statuses;
    mutable std::mutex status_mutex;
    std::atomic<bool> running{false};
    std::thread heartbeat_thread;
    std::map<std::string, std::unique_ptr<NetworkManager>> peer_connections;

    void handle_node_failure(const std::string& node_id);

public:
    HeartbeatManager() = default;
    void setDistributedNode(DistributedNode* node);

    void start();
    void heartbeat_worker(); 
    void stop();
    void register_node(const std::string& node_id);
    void update_heartbeat(const std::string& node_id);
    bool is_node_alive(const std::string& node_id) const;
    void add_peer_connection(const std::string& peer_id, std::unique_ptr<NetworkManager> manager);
};