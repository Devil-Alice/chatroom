#pragma once
#include "global.h"
#include "grpc_stub_pool.h"

class GrpcVerifyClient : public Singleton<GrpcVerifyClient>
{
    friend class Singleton<GrpcVerifyClient>;

private:
    // std::unique_ptr<VerifyService::Stub> stub_;
    std::unique_ptr<GrpcStubPool<my_grpc::Verify, my_grpc::Verify::Stub>> grpc_stub_pool_;
    GrpcVerifyClient();

public:
    // ~VerifyGrpcClient();
    GetVerifyResponse get_rerify_code(std::string phone);
};
