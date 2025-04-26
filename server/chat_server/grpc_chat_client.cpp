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

NotifySendFriendApplyResponse GrpcChatClient::notify_send_friend_apply(string server_host, const NotifySendFriendApplyRequest &request)
{
    NotifySendFriendApplyResponse response;

    return response;
}

NotifyHandleFriendApplyResponse GrpcChatClient::notify_handle_friend_apply(string server_host, const NotifyHandleFriendApplyRequest &request)
{
    NotifyHandleFriendApplyResponse response;

    return response;
}
