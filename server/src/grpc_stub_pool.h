#pragma once
#include <grpcpp/grpcpp.h>
#include "global.h"
#include "my_message.pb.h"
#include "my_message.grpc.pb.h"

template <typename GrpcService, typename GrpcStub>
class GrpcStubPool
{
private:
    size_t pool_size_;
    std::string host_;
    std::string port_;
    // stub时grpc通讯的代理对象，类似于connection
    std::queue<std::unique_ptr<GrpcStub>> stubs_;

    std::atomic<bool> stop_flag_;
    std::mutex mutex_;
    std::condition_variable cond_;

    void close()
    {
        stop_flag_ = true;
        // 唤醒通知
        cond_.notify_all();
    }

public:
    GrpcStubPool(size_t size, std::string host, std::string port) : pool_size_(size), host_(host), port_(port), stop_flag_(false)
    {
        for (size_t i = 0; i < pool_size_; i++)
        {
            std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());

            stubs_.push(GrpcService::NewStub(channel));

            // 以下是错误示范：
            //  std::unique_ptr<VerifyService::Stub> stub = VerifyService::NewStub(channel);
            //  stubs_.push(stub);
        }
    }

    ~GrpcStubPool()
    {
        std::lock_guard<std::mutex> locker(mutex_);
        close();
        while (!stubs_.empty())
        {
            stubs_.pop();
        }
    }

    std::unique_ptr<GrpcStub> get_grpc_stub()
    {
        std::unique_lock<std::mutex> locker(mutex_);
        cond_.wait(locker, [this]() -> bool
                   { return stop_flag_ || !stubs_.empty(); });

        if (stop_flag_)
            return nullptr;
        std::unique_ptr<GrpcStub> stub = std::move(stubs_.front());

        stubs_.pop();
        return stub;
    }

    void return_grpc_stub(std::unique_ptr<GrpcStub> stub)
    {
        std::lock_guard<std::mutex> locker(mutex_);
        if (stop_flag_)
            return;
        stubs_.push(std::move(stub));
        cond_.notify_one();
    }
};
