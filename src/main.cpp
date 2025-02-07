#include <iostream>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "network/network_manager.h"
#include "core/node.h"
#include "core/heartbeat_manager.h"
#include "storage/key_value_store.h"

void print_usage() {
    std::cout << "Usage: kvstore_node [OPTIONS]\n"
              << "Options:\n"
              << "  --node-id=<id>          Unique identifier for this node\n"
              << "  --address=<addr>        Address to listen on (e.g., 0.0.0.0:50051)\n"
              << "  --peers=<id:addr,...>   Comma-separated list of peer id:address pairs\n";
}

class KVStoreServiceImpl final : public kvstore::KVStoreService::Service {
private:
    DistributedNode& node;
    HeartbeatManager& heartbeat_mgr;

public:
    KVStoreServiceImpl(DistributedNode& n, HeartbeatManager& hb) 
        : node(n), heartbeat_mgr(hb) {}

    grpc::Status Put(grpc::ServerContext* /*context*/, 
                    const kvstore::PutRequest* request,
                    kvstore::PutResponse* response) override {
        node.put(request->key(), request->value());
        response->set_success(true);
        return grpc::Status::OK;
    }

    grpc::Status Get(grpc::ServerContext* /*context*/, 
                    const kvstore::GetRequest* request,
                    kvstore::GetResponse* response) override {
        auto result = node.get(request->key());
        response->set_found(result.has_value());
        if (result.has_value()) {
            response->set_value(result.value());
        }
        return grpc::Status::OK;
    }

    grpc::Status Heartbeat(grpc::ServerContext* /*context*/, 
                          const kvstore::HeartbeatRequest* request,
                          kvstore::HeartbeatResponse* response) override {
        heartbeat_mgr.update_heartbeat(request->node_id());
        response->set_acknowledged(true);
        return grpc::Status::OK;
    }
};

void run_server(const std::string& address, DistributedNode& node, HeartbeatManager& heartbeat_mgr) {
    KVStoreServiceImpl service(node, heartbeat_mgr);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << address << std::endl;
    server->Wait();
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    std::string node_id;
    std::string address;
    // Mapping from peer id to peer address.
    std::unordered_map<std::string, std::string> peers;

    // Parse command line arguments.
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.find("--node-id=") == 0) {
            node_id = arg.substr(10);
        } else if (arg.find("--address=") == 0) {
            address = arg.substr(10);
        } else if (arg.find("--peers=") == 0) {
            std::string peers_str = arg.substr(8);
            auto entries = split(peers_str, ',');
            for (const auto& entry : entries) {
                // Expect each entry in the format "peer_id:peer_address"
                auto pos = entry.find(':');
                if (pos != std::string::npos) {
                    std::string peer_id = entry.substr(0, pos);
                    std::string peer_address = entry.substr(pos + 1);
                    peers[peer_id] = peer_address;
                }
            }
        } else if (arg == "--help") {
            print_usage();
            return 0;
        }
    }

    if (node_id.empty() || address.empty()) {
        std::cerr << "Error: node-id and address are required\n";
        print_usage();
        return 1;
    }

    try {
        // Initialize local node and heartbeat manager.
        DistributedNode node(node_id, address);
        HeartbeatManager heartbeat_mgr;

        // Build a consistent hash ring including self and all peers (by node id).
        std::vector<std::string> all_nodes;
        all_nodes.push_back(node_id);
        for (const auto& [peer_id, peer_addr] : peers) {
            all_nodes.push_back(peer_id);
        }
        std::sort(all_nodes.begin(), all_nodes.end());

        // Register each peer in the hash ring and create their network managers.
        for (const auto& [peer_id, peer_addr] : peers) {
            if (peer_id != node_id) {
                node.addReplicaNode(peer_id, peer_addr);
                heartbeat_mgr.register_node(peer_id);
                heartbeat_mgr.add_peer_connection(peer_id, std::make_unique<NetworkManager>(peer_addr));
            }
        }

        // Start heartbeat manager.
        heartbeat_mgr.start();

        // Run the gRPC server in a separate thread.
        std::thread server_thread([&]() {
            run_server(address, node, heartbeat_mgr);
        });

        // Main interactive command loop.
        std::string command;
        while (std::getline(std::cin, command)) {
            if (command == "quit" || command == "exit")
                break;

            std::istringstream iss(command);
            std::string op;
            iss >> op;

            if (op == "put") {
                std::string key, value;
                if (iss >> key >> value) {
                    node.put(key, value);
                    std::cout << "Put: " << key << " -> " << value << std::endl;
                }
            } else if (op == "get") {
                std::string key;
                if (iss >> key) {
                    auto val = node.get(key);
                    if (val)
                        std::cout << "Get: " << key << " -> " << *val << std::endl;
                    else
                        std::cout << "Key not found: " << key << std::endl;
                }
            } else if (op == "status") {
                std::cout << "Node ID: " << node_id << "\nAddress: " << address << "\nPeers: ";
                for (const auto& [peer_id, peer_addr] : peers)
                    std::cout << peer_id << "(" << peer_addr << ") ";
                std::cout << std::endl;
            } else {
                std::cout << "Unknown command. Available commands: put, get, status, quit" << std::endl;
            }
        }

        // Clean up.
        heartbeat_mgr.stop();
        server_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}