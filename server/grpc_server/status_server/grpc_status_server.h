#pragma once
#include <my_grpc.pb.h>
#include <my_grpc.grpc.pb.h>
#include <vector>
#include "chat_server.h"

using my_grpc::GetChatServerRequest;
using my_grpc::GetChatServerResponse;


class GrpcStatusServer final : public my_grpc::Status::Service
{
private:
    std::vector<ChatServer> chat_servers_;
    int poll_idx_;
public:
    GrpcStatusServer();
    ~GrpcStatusServer();
    grpc::Status get_chat_server(grpc::ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response);
};