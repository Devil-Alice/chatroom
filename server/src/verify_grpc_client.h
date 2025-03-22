#pragma once
#include "global.h"
#include "grpc_stub_pool.h"

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;

private:
    // std::unique_ptr<VerifyService::Stub> stub_;
    std::unique_ptr<GrpcStubPool<VerifyService, VerifyService::Stub>> grpc_stub_pool;
    VerifyGrpcClient();

public:
    // ~VerifyGrpcClient();
    GetVerifyResponse get_rerify_code(std::string phone);
};
