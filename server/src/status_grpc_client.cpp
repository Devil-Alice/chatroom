#include "status_grpc_client.h"

StatusGrpcClient::StatusGrpcClient()
{
    string host = app_config["status_server"]["host"];
    string port = app_config["status_server"]["port"];
    grpc_stub_pool_.reset(new GrpcStubPool<StatusService, StatusService::Stub>(10, host, port));

}

StatusGrpcClient::~StatusGrpcClient()
{

}

GetChatServerResponse StatusGrpcClient::get_chat_server(int uid)
{
    auto conn = grpc_stub_pool_->get_grpc_stub();

    ClientContext context;
    GetChatServerRequest request;
    GetChatServerResponse response;

    request.set_uid(uid);

    grpc::Status status = conn->get_chat_server(&context, request, &response);
    if (!status.ok())
    {
        std::cout << "get_chat_server error: " << status.error_code() << std::endl;
        response.set_error(MY_STATUS_CODE::RPC_FAILED);
    }

    grpc_stub_pool_->return_grpc_stub(std::move(conn));
    return response;
}
