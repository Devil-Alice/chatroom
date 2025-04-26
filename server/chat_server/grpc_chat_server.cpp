#include "grpc_chat_server.h"

grpc::Status GrpcChatServer::notify_send_friend_apply(grpc::ServerContext *context, const NotifySendFriendApplyRequest *request, NotifySendFriendApplyResponse *response)
{

    return grpc::Status::OK;
}

grpc::Status GrpcChatServer::notify_handle_friend_apply(grpc::ServerContext *context, const NotifyHandleFriendApplyRequest *request, NotifyHandleFriendApplyResponse *response)
{
    return grpc::Status();
}
