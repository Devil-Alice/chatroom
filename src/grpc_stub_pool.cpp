#include "grpc_stub_pool.h"

GrpcStubPool::GrpcStubPool(size_t size, std::string host, std::string port) : pool_size_(size), host_(host), port_(port), stop_flag_(false)
{
    for (size_t i = 0; i < pool_size_; i++)
    {
        std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
        
        stubs_.push(VerifyService::NewStub(channel));

        // 以下是错误示范：
        //  std::unique_ptr<VerifyService::Stub> stub = VerifyService::NewStub(channel);
        //  stubs_.push(stub);
    }
}

void GrpcStubPool::close()
{
    stop_flag_ = true;
    // 唤醒通知
    cond_.notify_all();
}

GrpcStubPool::~GrpcStubPool()
{
    std::lock_guard<std::mutex> locker(mutex_);
    close();
    while (!stubs_.empty())
    {
        stubs_.pop();
    }
}

std::unique_ptr<VerifyService::Stub> GrpcStubPool::get_grpc_stub()
{
    std::unique_lock<std::mutex> locker(mutex_);
    cond_.wait(locker, [this]() -> bool
               {
        if (stop_flag_)
            return false;
        return !stubs_.empty(); });

    std::unique_ptr<VerifyService::Stub> stub = std::move(stubs_.front());

    stubs_.pop();
    return stub;
}

void GrpcStubPool::return_grpc_stub(std::unique_ptr<VerifyService::Stub> stub)
{
    std::lock_guard<std::mutex> locker(mutex_);
    if (stop_flag_)
        return;
    stubs_.push(std::move(stub));
    cond_.notify_one();
}
