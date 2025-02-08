#pragma once
#include <grpcpp/grpcpp.h>
#include "rpc_service.pb.h"
#include "rpc_service.grpc.pb.h"
#include <memory>
#include <string>
#include <optional>

class NetworkManager {
private:
    std::unique_ptr<kvstore::KVStoreService::Stub> stub_;
    std::shared_ptr<grpc::Channel> channel_;

public:
    NetworkManager(const std::string& target_address);
    bool put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    bool send_heartbeat(const std::string& node_id);
    kvstore::KVStoreService::Stub* getStub() { return stub_.get(); }
};

