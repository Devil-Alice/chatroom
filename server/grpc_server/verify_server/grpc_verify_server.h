#pragma once
#include <grpcpp/grpcpp.h>
#include "my_grpc.pb.h"
#include "my_grpc.grpc.pb.h"

using grpc::ServerContext;
using grpc::Service;
using grpc::Status;

using my_grpc::GetVerifyRequest;
using my_grpc::GetVerifyResponse;


class GrpcVerifyServer final: public my_grpc::Verify::Service
{
private:
public:
    Status get_verify_code(ServerContext *context, const GetVerifyRequest *req, GetVerifyResponse *rsp) override;
};