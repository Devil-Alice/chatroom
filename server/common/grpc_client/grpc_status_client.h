#include "global.h"
#include "grpc_stub_pool.h"

class GrpcStatusClient : public Singleton<GrpcStatusClient>
{
    friend class Singleton<GrpcStatusClient>;
    using string = std::string;
private:
    std::unique_ptr<GrpcStubPool<my_grpc::Status, my_grpc::Status::Stub>> grpc_stub_pool_;

public:
    GrpcStatusClient();
    ~GrpcStatusClient();
    GetChatServerResponse get_chat_server(string uid);
    UserLoginResponse user_login(string uid, string token);
};