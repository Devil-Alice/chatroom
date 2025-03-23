// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: my_grpc.proto

#include "my_grpc.pb.h"
#include "my_grpc.grpc.pb.h"

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
namespace my_grpc {

static const char* Verify_method_names[] = {
  "/my_grpc.Verify/get_verify_code",
};

std::unique_ptr< Verify::Stub> Verify::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Verify::Stub> stub(new Verify::Stub(channel, options));
  return stub;
}

Verify::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_get_verify_code_(Verify_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Verify::Stub::get_verify_code(::grpc::ClientContext* context, const ::my_grpc::GetVerifyRequest& request, ::my_grpc::GetVerifyResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::my_grpc::GetVerifyRequest, ::my_grpc::GetVerifyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_get_verify_code_, context, request, response);
}

void Verify::Stub::async::get_verify_code(::grpc::ClientContext* context, const ::my_grpc::GetVerifyRequest* request, ::my_grpc::GetVerifyResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::my_grpc::GetVerifyRequest, ::my_grpc::GetVerifyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_verify_code_, context, request, response, std::move(f));
}

void Verify::Stub::async::get_verify_code(::grpc::ClientContext* context, const ::my_grpc::GetVerifyRequest* request, ::my_grpc::GetVerifyResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_verify_code_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::my_grpc::GetVerifyResponse>* Verify::Stub::PrepareAsyncget_verify_codeRaw(::grpc::ClientContext* context, const ::my_grpc::GetVerifyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::my_grpc::GetVerifyResponse, ::my_grpc::GetVerifyRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_get_verify_code_, context, request);
}

::grpc::ClientAsyncResponseReader< ::my_grpc::GetVerifyResponse>* Verify::Stub::Asyncget_verify_codeRaw(::grpc::ClientContext* context, const ::my_grpc::GetVerifyRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncget_verify_codeRaw(context, request, cq);
  result->StartCall();
  return result;
}

Verify::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Verify_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Verify::Service, ::my_grpc::GetVerifyRequest, ::my_grpc::GetVerifyResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Verify::Service* service,
             ::grpc::ServerContext* ctx,
             const ::my_grpc::GetVerifyRequest* req,
             ::my_grpc::GetVerifyResponse* resp) {
               return service->get_verify_code(ctx, req, resp);
             }, this)));
}

Verify::Service::~Service() {
}

::grpc::Status Verify::Service::get_verify_code(::grpc::ServerContext* context, const ::my_grpc::GetVerifyRequest* request, ::my_grpc::GetVerifyResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* Status_method_names[] = {
  "/my_grpc.Status/get_chat_server",
};

std::unique_ptr< Status::Stub> Status::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Status::Stub> stub(new Status::Stub(channel, options));
  return stub;
}

Status::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_get_chat_server_(Status_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Status::Stub::get_chat_server(::grpc::ClientContext* context, const ::my_grpc::GetChatServerRequest& request, ::my_grpc::GetChatServerResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::my_grpc::GetChatServerRequest, ::my_grpc::GetChatServerResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_get_chat_server_, context, request, response);
}

void Status::Stub::async::get_chat_server(::grpc::ClientContext* context, const ::my_grpc::GetChatServerRequest* request, ::my_grpc::GetChatServerResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::my_grpc::GetChatServerRequest, ::my_grpc::GetChatServerResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_chat_server_, context, request, response, std::move(f));
}

void Status::Stub::async::get_chat_server(::grpc::ClientContext* context, const ::my_grpc::GetChatServerRequest* request, ::my_grpc::GetChatServerResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_get_chat_server_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::my_grpc::GetChatServerResponse>* Status::Stub::PrepareAsyncget_chat_serverRaw(::grpc::ClientContext* context, const ::my_grpc::GetChatServerRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::my_grpc::GetChatServerResponse, ::my_grpc::GetChatServerRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_get_chat_server_, context, request);
}

::grpc::ClientAsyncResponseReader< ::my_grpc::GetChatServerResponse>* Status::Stub::Asyncget_chat_serverRaw(::grpc::ClientContext* context, const ::my_grpc::GetChatServerRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncget_chat_serverRaw(context, request, cq);
  result->StartCall();
  return result;
}

Status::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Status_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Status::Service, ::my_grpc::GetChatServerRequest, ::my_grpc::GetChatServerResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Status::Service* service,
             ::grpc::ServerContext* ctx,
             const ::my_grpc::GetChatServerRequest* req,
             ::my_grpc::GetChatServerResponse* resp) {
               return service->get_chat_server(ctx, req, resp);
             }, this)));
}

Status::Service::~Service() {
}

::grpc::Status Status::Service::get_chat_server(::grpc::ServerContext* context, const ::my_grpc::GetChatServerRequest* request, ::my_grpc::GetChatServerResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace my_grpc

