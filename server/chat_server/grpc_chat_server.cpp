#include "grpc_chat_server.h"
#include "user_manager.h"

grpc::Status GrpcChatServer::notify(grpc::ServerContext *context, const NotifyRequest *request, NotifyResponse *response)
{
    response->set_error(MY_STATUS_CODE::ERROR);
    response->set_uid(request->from_uid());
    response->set_token(request->token());

    
    
    // 获取数据包内容
    string from_uid = request->from_uid();
    string token = request->token();
    string to_uid = request->to_uid();
    int request_id = request->request_id();
    string message = request->message();

    std::cout << "rpc server received a notify request: " << std::endl
            << "\t from: " << from_uid << std::endl
            << "\t to: " << to_uid << std::endl
            << "\t request_id: " << request << std::endl
            << "\t message: " << message << std::endl;

    auto session = UserManager::instance().get_uid_session(to_uid);
    if (session == nullptr)
    {
        // 这里未找到，说明用户不在线，不做提醒
        return grpc::Status::OK;
    }

    std::shared_ptr<Package> package(new Package());
    package->set_request_id(request_id);
    package->set_message(message);
    session->add_response(package);

    return grpc::Status::OK;
}

grpc::Status GrpcChatServer::notify_send_friend_apply(grpc::ServerContext *context, const NotifySendFriendApplyRequest *request, NotifySendFriendApplyResponse *response)
{

    return grpc::Status::OK;
}

grpc::Status GrpcChatServer::notify_handle_friend_apply(grpc::ServerContext *context, const NotifyHandleFriendApplyRequest *request, NotifyHandleFriendApplyResponse *response)
{
    return grpc::Status();
}
