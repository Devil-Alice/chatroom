#pragma once
#include "global.h"
#include "grpc_stub_pool.h"

class GrpcChatClient : public Singleton<GrpcChatClient>
{
    friend class Singleton<GrpcChatClient>;
    using string = std::string;
    using pool_ptr = std::shared_ptr<GrpcStubPool<my_grpc::Chat, my_grpc::Chat::Stub>>;

private:
    // map key为name，value是
    std::map<string, pool_ptr> grpc_stub_pools_;

public:
    GrpcChatClient();
    NotifySendFriendApplyResponse notify_send_friend_apply(string server_host, const NotifySendFriendApplyRequest &request);
    NotifyHandleFriendApplyResponse notify_handle_friend_apply(string server_host, const NotifyHandleFriendApplyRequest &request);
};
