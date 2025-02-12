// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: rpc_service.proto

#include "rpc_service.pb.h"
#include "rpc_service.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace kvstore {

static const char* KVStoreService_method_names[] = {
  "/kvstore.KVStoreService/Put",
  "/kvstore.KVStoreService/Get",
  "/kvstore.KVStoreService/Heartbeat",
  "/kvstore.KVStoreService/Join",
};

std::unique_ptr< KVStoreService::Stub> KVStoreService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< KVStoreService::Stub> stub(new KVStoreService::Stub(channel, options));
  return stub;
}

KVStoreService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Put_(KVStoreService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Get_(KVStoreService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Heartbeat_(KVStoreService_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Join_(KVStoreService_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status KVStoreService::Stub::Put(::grpc::ClientContext* context, const ::kvstore::PutRequest& request, ::kvstore::PutResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::kvstore::PutRequest, ::kvstore::PutResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Put_, context, request, response);
}

void KVStoreService::Stub::async::Put(::grpc::ClientContext* context, const ::kvstore::PutRequest* request, ::kvstore::PutResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::kvstore::PutRequest, ::kvstore::PutResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Put_, context, request, response, std::move(f));
}

void KVStoreService::Stub::async::Put(::grpc::ClientContext* context, const ::kvstore::PutRequest* request, ::kvstore::PutResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Put_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::kvstore::PutResponse>* KVStoreService::Stub::PrepareAsyncPutRaw(::grpc::ClientContext* context, const ::kvstore::PutRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::kvstore::PutResponse, ::kvstore::PutRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Put_, context, request);
}

::grpc::ClientAsyncResponseReader< ::kvstore::PutResponse>* KVStoreService::Stub::AsyncPutRaw(::grpc::ClientContext* context, const ::kvstore::PutRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncPutRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status KVStoreService::Stub::Get(::grpc::ClientContext* context, const ::kvstore::GetRequest& request, ::kvstore::GetResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::kvstore::GetRequest, ::kvstore::GetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Get_, context, request, response);
}

void KVStoreService::Stub::async::Get(::grpc::ClientContext* context, const ::kvstore::GetRequest* request, ::kvstore::GetResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::kvstore::GetRequest, ::kvstore::GetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Get_, context, request, response, std::move(f));
}

void KVStoreService::Stub::async::Get(::grpc::ClientContext* context, const ::kvstore::GetRequest* request, ::kvstore::GetResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Get_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::kvstore::GetResponse>* KVStoreService::Stub::PrepareAsyncGetRaw(::grpc::ClientContext* context, const ::kvstore::GetRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::kvstore::GetResponse, ::kvstore::GetRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Get_, context, request);
}

::grpc::ClientAsyncResponseReader< ::kvstore::GetResponse>* KVStoreService::Stub::AsyncGetRaw(::grpc::ClientContext* context, const ::kvstore::GetRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status KVStoreService::Stub::Heartbeat(::grpc::ClientContext* context, const ::kvstore::HeartbeatRequest& request, ::kvstore::HeartbeatResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::kvstore::HeartbeatRequest, ::kvstore::HeartbeatResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Heartbeat_, context, request, response);
}

void KVStoreService::Stub::async::Heartbeat(::grpc::ClientContext* context, const ::kvstore::HeartbeatRequest* request, ::kvstore::HeartbeatResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::kvstore::HeartbeatRequest, ::kvstore::HeartbeatResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Heartbeat_, context, request, response, std::move(f));
}

void KVStoreService::Stub::async::Heartbeat(::grpc::ClientContext* context, const ::kvstore::HeartbeatRequest* request, ::kvstore::HeartbeatResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Heartbeat_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::kvstore::HeartbeatResponse>* KVStoreService::Stub::PrepareAsyncHeartbeatRaw(::grpc::ClientContext* context, const ::kvstore::HeartbeatRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::kvstore::HeartbeatResponse, ::kvstore::HeartbeatRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Heartbeat_, context, request);
}

::grpc::ClientAsyncResponseReader< ::kvstore::HeartbeatResponse>* KVStoreService::Stub::AsyncHeartbeatRaw(::grpc::ClientContext* context, const ::kvstore::HeartbeatRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncHeartbeatRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status KVStoreService::Stub::Join(::grpc::ClientContext* context, const ::kvstore::JoinRequest& request, ::kvstore::JoinResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::kvstore::JoinRequest, ::kvstore::JoinResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Join_, context, request, response);
}

void KVStoreService::Stub::async::Join(::grpc::ClientContext* context, const ::kvstore::JoinRequest* request, ::kvstore::JoinResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::kvstore::JoinRequest, ::kvstore::JoinResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Join_, context, request, response, std::move(f));
}

void KVStoreService::Stub::async::Join(::grpc::ClientContext* context, const ::kvstore::JoinRequest* request, ::kvstore::JoinResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Join_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::kvstore::JoinResponse>* KVStoreService::Stub::PrepareAsyncJoinRaw(::grpc::ClientContext* context, const ::kvstore::JoinRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::kvstore::JoinResponse, ::kvstore::JoinRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Join_, context, request);
}

::grpc::ClientAsyncResponseReader< ::kvstore::JoinResponse>* KVStoreService::Stub::AsyncJoinRaw(::grpc::ClientContext* context, const ::kvstore::JoinRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncJoinRaw(context, request, cq);
  result->StartCall();
  return result;
}

KVStoreService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KVStoreService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KVStoreService::Service, ::kvstore::PutRequest, ::kvstore::PutResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](KVStoreService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::kvstore::PutRequest* req,
             ::kvstore::PutResponse* resp) {
               return service->Put(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KVStoreService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KVStoreService::Service, ::kvstore::GetRequest, ::kvstore::GetResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](KVStoreService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::kvstore::GetRequest* req,
             ::kvstore::GetResponse* resp) {
               return service->Get(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KVStoreService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KVStoreService::Service, ::kvstore::HeartbeatRequest, ::kvstore::HeartbeatResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](KVStoreService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::kvstore::HeartbeatRequest* req,
             ::kvstore::HeartbeatResponse* resp) {
               return service->Heartbeat(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KVStoreService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KVStoreService::Service, ::kvstore::JoinRequest, ::kvstore::JoinResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](KVStoreService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::kvstore::JoinRequest* req,
             ::kvstore::JoinResponse* resp) {
               return service->Join(ctx, req, resp);
             }, this)));
}

KVStoreService::Service::~Service() {
}

::grpc::Status KVStoreService::Service::Put(::grpc::ServerContext* context, const ::kvstore::PutRequest* request, ::kvstore::PutResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status KVStoreService::Service::Get(::grpc::ServerContext* context, const ::kvstore::GetRequest* request, ::kvstore::GetResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status KVStoreService::Service::Heartbeat(::grpc::ServerContext* context, const ::kvstore::HeartbeatRequest* request, ::kvstore::HeartbeatResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status KVStoreService::Service::Join(::grpc::ServerContext* context, const ::kvstore::JoinRequest* request, ::kvstore::JoinResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace kvstore

