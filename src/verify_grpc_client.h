#pragma once
#include "global.h"
#include <grpcpp/grpcpp.h>
#include "my_message.pb.h"
#include "my_message.grpc.pb.h"

// 引入作用域
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using my_message::GetVerifyRequest;
using my_message::GetVerifyResponse;
using my_message::VerifyService;

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;

private:
    // 在 gRPC 中，stub（存根）是 客户端用于调用 RPC 方法的代理对象，它封装了网络通信的细节，使得客户端可以像调用本地函数一样调用远程服务
    std::unique_ptr<VerifyService::Stub> stub_;

private:
    VerifyGrpcClient()
    {
        // 初始化channel，初始化stub
        std::string server_address = "127.0.0.1:" + app_config["rpc_server"]["port"];
        std::shared_ptr<Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
        stub_ = VerifyService::NewStub(channel);
    }

public:
    // ~VerifyGrpcClient();
    GetVerifyResponse get_rerify_code(std::string phone)
    {
        ClientContext context;
        GetVerifyRequest req;
        GetVerifyResponse rsp;

        req.set_phone(phone);
        Status status = stub_->get_verify_code(&context, req, &rsp);
        if (status.ok())
            return rsp;
        else
            rsp.set_error(MY_ERROR_CODE::RPC_RAILED);

        return rsp;
    }
};
