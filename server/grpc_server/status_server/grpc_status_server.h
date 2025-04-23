#pragma once
#include <my_grpc.pb.h>
#include <my_grpc.grpc.pb.h>
#include <vector>
#include "global.h"

using my_grpc::GetChatServerRequest;
using my_grpc::GetChatServerResponse;

using my_grpc::UserLoginRequest;
using my_grpc::UserLoginResponse;


class GrpcStatusServer final : public my_grpc::Status::Service
{
    using string = std::string;
private:
    std::mutex mutex_;
    std::vector<ChatServerInfo> chat_servers_infos_;
    int poll_idx_;
    // key是uid，value时token，优化后，将tokens存入redis
    // std::map<string, string> tokens_;
public:
    GrpcStatusServer();
    ~GrpcStatusServer();
    grpc::Status get_chat_server(grpc::ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response);
    grpc::Status user_login(grpc::ServerContext *context, const UserLoginRequest *request, UserLoginResponse *response);
};