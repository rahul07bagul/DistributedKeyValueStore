#include "network_manager.h"

NetworkManager::NetworkManager(const std::string& target_address) {
    channel_ = grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials());
    stub_ = kvstore::KVStoreService::NewStub(channel_);
}

bool NetworkManager::put(const std::string& key, const std::string& value) {
    grpc::ClientContext context;
    kvstore::PutRequest request;
    kvstore::PutResponse response;

    request.set_key(key);
    request.set_value(value);

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
    context.set_deadline(deadline);

    grpc::Status status = stub_->Put(&context, request, &response);
    return status.ok() && response.success();
}

std::optional<std::string> NetworkManager::get(const std::string& key) {
    grpc::ClientContext context;
    kvstore::GetRequest request;
    kvstore::GetResponse response;

    request.set_key(key);

    std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(500);
    context.set_deadline(deadline);

    grpc::Status status = stub_->Get(&context, request, &response);

    if (!status.ok()) {
        std::cerr << "[WARN] gRPC Get request to replica failed: " << status.error_message() << std::endl;
        return std::nullopt; 
    }

    return response.found() ? std::optional<std::string>(response.value()) : std::nullopt;
}

bool NetworkManager::send_heartbeat(const std::string& node_id) {
    grpc::ClientContext context;
    kvstore::HeartbeatRequest request;
    kvstore::HeartbeatResponse response;

    request.set_node_id(node_id);

    grpc::Status status = stub_->Heartbeat(&context, request, &response);
    return status.ok() && response.acknowledged();
}