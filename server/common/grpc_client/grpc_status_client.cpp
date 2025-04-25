#include "grpc_status_client.h"

GrpcStatusClient::GrpcStatusClient()
{
    string host = app_config["status_server"]["host"];
    string port = app_config["status_server"]["port"];
    grpc_stub_pool_.reset(new GrpcStubPool<my_grpc::Status, my_grpc::Status::Stub>(10, host, port));

}

GrpcStatusClient::~GrpcStatusClient()
{

}

GetChatServerResponse GrpcStatusClient::get_chat_server(string uid)
{
    auto conn = grpc_stub_pool_->get_grpc_stub();

    ClientContext context;
    GetChatServerRequest request;
    GetChatServerResponse response;

    request.set_uid(uid);

    grpc::Status status = conn->get_chat_server(&context, request, &response);
    if (!status.ok())
    {
        response.set_error(MY_STATUS_CODE::RPC_FAILED);
        std::cout << "get_chat_server error: " << status.error_code() << std::endl;
    }

    grpc_stub_pool_->return_grpc_stub(std::move(conn));
    return response;
}

UserLoginResponse GrpcStatusClient::chat_login(string uid, string token, string server_name)
{
    ClientContext context;
    UserLoginRequest request;
    UserLoginResponse response;

    request.set_uid(uid);
    request.set_token(token);
    request.set_server_name(server_name);

    auto stub = grpc_stub_pool_->get_grpc_stub();
    grpc::Status status = stub->chat_login(&context, request, &response);

    if (!status.ok())
    {
        response.set_error(MY_STATUS_CODE::RPC_FAILED);
        std::cout << "user_login error: " << status.error_code() << std::endl;
    }

    grpc_stub_pool_->return_grpc_stub(std::move(stub));
    return UserLoginResponse();
}
