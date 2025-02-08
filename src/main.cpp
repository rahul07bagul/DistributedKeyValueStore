#include <iostream>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "network/network_manager.h"
#include "core/node.h"
#include "core/heartbeat_manager.h"
#include "storage/key_value_store.h"
#include "core/command_handler.h"
#include "core/input_parser.h"
#include "core/node_initializer.h"

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

int main(int argc, char* argv[]) {
    InputParser parser(argc, argv);
    auto parsed_input = parser.parse();
    if (!parsed_input) {
        return 1;
    }

    try {
        auto context = NodeInitializer::initialize(*parsed_input);

        std::thread server_thread([&]() {
            run_server(parsed_input->address, *context->node, *context->heartbeat_mgr);
        });

        std::string command;
        while (std::getline(std::cin, command)) {
            if (!context->cmd_handler->handleCommand(command))
                break;
        }

        // Clean up
        context->heartbeat_mgr->stop();
        server_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}