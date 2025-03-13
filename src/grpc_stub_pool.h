#pragma once
#include <grpcpp/grpcpp.h>
#include "global.h"
#include "my_message.pb.h"
#include "my_message.grpc.pb.h"

class GrpcStubPool
{
private:
    size_t pool_size_;
    std::string host_;
    std::string port_;
    // stub时grpc通讯的代理对象，类似于connection
    std::queue<std::unique_ptr<VerifyService::Stub>> stubs_;

    std::atomic<bool> stop_flag_;
    std::mutex mutex_;
    std::condition_variable cond_;

    void close();

public:
    GrpcStubPool(size_t size, std::string host, std::string port);
    ~GrpcStubPool();
    std::unique_ptr<VerifyService::Stub> get_grpc_stub();
    void return_grpc_stub(std::unique_ptr<VerifyService::Stub> stub);
};