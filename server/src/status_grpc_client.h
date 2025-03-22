#include "global.h"
#include "grpc_stub_pool.h"

class StatusGrpcClient : Singleton<StatusGrpcClient>
{
    friend class Singleton<StatusGrpcClient>;
    using string = std::string;
private:
    std::unique_ptr<GrpcStubPool<StatusService, StatusService::Stub>> grpc_stub_pool_;

public:
    StatusGrpcClient();
    ~StatusGrpcClient();
    GetChatServerResponse get_chat_server(int uid);

};