#include "global.h"
#include "grpc_stub_pool.h"

class GrpcStatusClient : Singleton<GrpcStatusClient>
{
    friend class Singleton<GrpcStatusClient>;
    using string = std::string;
private:
    std::unique_ptr<GrpcStubPool<my_grpc::Status, my_grpc::Status::Stub>> grpc_stub_pool_;

public:
    GrpcStatusClient();
    ~GrpcStatusClient();
    GetChatServerResponse get_chat_server(int uid);

};