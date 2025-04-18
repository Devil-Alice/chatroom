#include "grpc_verify_client.h"

GrpcVerifyClient::GrpcVerifyClient()
{
    std::string host = app_config["verify_server"]["host"];
    std::string port = app_config["verify_server"]["port"];
    grpc_stub_pool_.reset(new GrpcStubPool<my_grpc::Verify, my_grpc::Verify::Stub>(10, host, port));
}

GetVerifyResponse GrpcVerifyClient::get_rerify_code(std::string phone)
{
    ClientContext context;
    GetVerifyRequest req;
    GetVerifyResponse rsp;

    req.set_phone(phone);
    // 在 gRPC 中，stub（存根）是 客户端用于调用 RPC 方法的代理对象，它封装了网络通信的细节，使得客户端可以像调用本地函数一样调用远程服务
    // 获取stub
    std::unique_ptr<my_grpc::Verify::Stub> stub = grpc_stub_pool_->get_grpc_stub();

    Status status = stub->get_verify_code(&context, req, &rsp);
    if (!status.ok())
        rsp.set_error(MY_STATUS_CODE::RPC_FAILED);

    // 返还stub
    grpc_stub_pool_->return_grpc_stub(std::move(stub));
    return rsp;
}