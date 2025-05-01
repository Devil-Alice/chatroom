#pragma once
#include "global.h"

class GrpcChatServer : public my_grpc::Chat::Service, public Singleton<GrpcChatServer>
{
    friend class Singleton<GrpcChatServer>;
    using string = std::string;

public:
    grpc::Status notify(grpc::ServerContext *context, const NotifyRequest *request, NotifyResponse *response) override;
    grpc::Status notify_send_friend_apply(grpc::ServerContext *context, const NotifySendFriendApplyRequest *request, NotifySendFriendApplyResponse *response);
    grpc::Status notify_handle_friend_apply(grpc::ServerContext *context, const NotifyHandleFriendApplyRequest *request, NotifyHandleFriendApplyResponse *response);
};