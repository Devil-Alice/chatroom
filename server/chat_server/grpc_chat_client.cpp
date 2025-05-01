#include "grpc_chat_client.h"

GrpcChatClient::GrpcChatClient()
{
    // 解析多个对端服务器的配置，每个建立一个pool
    string other_servers_str = app_config["chat_servers"].get_str();

    Json::Value json_array = JsonObject::parse_json_string(other_servers_str);

    // todo: 这里如果希望能保存同一host的stub，map的key就不能存host，否则会被覆盖！！！！
    for (auto item : json_array)
    {
        string name = item["name"].asString();
        // 在解析时排除自身
        if (name == app_config["chat_server_serlf"]["name"])
            continue;

        string host = item["host"].asString();
        string grpc_chat_server_port = item["grpc_chat_server_port"].asString();

        grpc_stub_pools_[name].reset(new GrpcStubPool<my_grpc::Chat, my_grpc::Chat::Stub>(10, host, grpc_chat_server_port));
    }
}

NotifyResponse GrpcChatClient::notify(string server_name, const NotifyRequest &request)
{
    NotifyResponse response;
    response.set_error(MY_STATUS_CODE::RPC_FAILED);

    if (grpc_stub_pools_.find(server_name) == grpc_stub_pools_.end())
    {
        return response;
    }

    auto pool = grpc_stub_pools_[server_name];
    auto conn = pool->get_grpc_stub();

    // 向grpc服务器发送提醒的请求
    grpc::ClientContext context;
    auto status = conn->notify(&context, request, &response);

    if (!status.ok())
    {
        std::cout << "GrpcChatClient::notify error details: " << status.error_details() << std::endl
                    << "GrpcChatClient::notify error message: " << status.error_message() << std::endl;
        response.set_error(MY_STATUS_CODE::RPC_FAILED);
        return response;
    }

    pool->return_grpc_stub(std::move(conn));
    return response;
}

NotifySendFriendApplyResponse GrpcChatClient::notify_send_friend_apply(string server_name, const NotifySendFriendApplyRequest &request)
{
    NotifySendFriendApplyResponse response;

    return response;
}

NotifyHandleFriendApplyResponse GrpcChatClient::notify_handle_friend_apply(string server_name, const NotifyHandleFriendApplyRequest &request)
{
    NotifyHandleFriendApplyResponse response;

    return response;
}
