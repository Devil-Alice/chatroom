#pragma once
#include <grpcpp/grpcpp.h>
#include "my_message.pb.h"
#include "my_message.grpc.pb.h"

using grpc::ServerContext;
using grpc::Service;
using grpc::Status;

using my_message::GetVerifyRequest;
using my_message::GetVerifyResponse;
using my_message::VerifyService;

class VerifyGrpcServer : public VerifyService::Service
{
private:
public:
    Status get_verify_code(ServerContext *context, const GetVerifyRequest *req, GetVerifyResponse *rsp) override;
};