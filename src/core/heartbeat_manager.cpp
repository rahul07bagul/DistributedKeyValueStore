#include "heartbeat_manager.h"
#include <iostream>

void HeartbeatManager::start() {
    running = true;
    heartbeat_thread = std::thread(&HeartbeatManager::heartbeat_worker, this);
}

// void HeartbeatManager::heartbeat_worker() {
//     while (running) {
//         for (const auto& [peer_id, manager] : peer_connections) {
//             if (!manager->send_heartbeat(peer_id)) {
//                 handle_node_failure(peer_id);
//             }
//         }
//         std::this_thread::sleep_for(std::chrono::seconds(2));
//     }
// }

void HeartbeatManager::heartbeat_worker() {
    while (running) {
        // Take a snapshot of the current peer IDs.
        std::vector<std::string> peer_ids;
        {
            std::lock_guard<std::mutex> lock(status_mutex);
            for (const auto& [peer_id, manager] : peer_connections) {
                peer_ids.push_back(peer_id);
            }
        }
        
        // Iterate over the snapshot.
        for (const auto& peer_id : peer_ids) {
            std::unique_ptr<NetworkManager>* manager_ptr = nullptr;
            {
                std::lock_guard<std::mutex> lock(status_mutex);
                auto it = peer_connections.find(peer_id);
                if (it != peer_connections.end()) {
                    manager_ptr = &(it->second);
                }
            }
            if (manager_ptr && !(*manager_ptr)->send_heartbeat(peer_id)) {
                // Handle failure outside of the locked region.
                handle_node_failure(peer_id);
            }
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Adjust as needed.
    }
}

void HeartbeatManager::stop() {
    running = false;
    if (heartbeat_thread.joinable()) {
        heartbeat_thread.join();
    }
}

void HeartbeatManager::register_node(const std::string& node_id) {
    std::lock_guard<std::mutex> lock(status_mutex);
    node_statuses[node_id] = {
        std::chrono::steady_clock::now(),
        true
    };
    std::cout << "Registered node: " << node_id << std::endl;
}

void HeartbeatManager::update_heartbeat(const std::string& node_id) {
    std::lock_guard<std::mutex> lock(status_mutex);
    auto it = node_statuses.find(node_id);
    if (it != node_statuses.end()) {
        it->second.last_heartbeat = std::chrono::steady_clock::now();
        it->second.is_alive = true;
    }
}

bool HeartbeatManager::is_node_alive(const std::string& node_id) const {
    std::lock_guard<std::mutex> lock(status_mutex);
    auto it = node_statuses.find(node_id);
    return it != node_statuses.end() && it->second.is_alive;
}

void HeartbeatManager::handle_node_failure(const std::string& node_id) {
    {
        std::lock_guard<std::mutex> lock(status_mutex);
        std::cout << "Node failure detected: " << node_id << std::endl;
        
        auto status_it = node_statuses.find(node_id);
        if (status_it != node_statuses.end()) {
            status_it->second.is_alive = false;
        }
        
        auto peer_it = peer_connections.find(node_id);
        if (peer_it != peer_connections.end()) {
            peer_connections.erase(peer_it);
        }
    }
    
    if (on_node_failure_callback) {
        on_node_failure_callback(node_id);
    }
}

void HeartbeatManager::add_peer_connection(const std::string& peer_id, std::unique_ptr<NetworkManager> manager) {
    peer_connections[peer_id] = std::move(manager);
}