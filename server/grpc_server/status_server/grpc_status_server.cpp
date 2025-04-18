#include "grpc_status_server.h"

GrpcStatusServer::GrpcStatusServer()
{
    poll_idx_ = 0;

    // 从配置文件中读取服务器信息
    string servers = app_config["chat_servers"].get_str();
    Json::Value root = JsonObject::parse_json_string(servers);
    if (!root.isArray())
    {
        std::cout << "parse chat servers error" << std::endl;
    }

    for (auto it = root.begin(); it != root.end(); it++)
    {
        ChatServerInfo info;
        info.host = it->get("host", "").asString();
        info.port = it->get("port", 0).asInt();
        std::cout << info.host << " " << info.port << std::endl;
        chat_servers_infos_.push_back(info);
    }

}

GrpcStatusServer::~GrpcStatusServer()
{
}

grpc::Status GrpcStatusServer::get_chat_server(grpc::ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response)
{
    // chat_servers_infos_是公共资源，而grpc处理多个任务是多线程，所以需要加锁
    std::lock_guard<std::mutex> locker(mutex_);
    ChatServerInfo server = chat_servers_infos_[poll_idx_];
    // 轮询获取，更新下标
    poll_idx_ = (poll_idx_ + 1) % chat_servers_infos_.size();

    // 用户请求聊天服务器的时候，给他生成一个token，传回
    string token = my_utils::generate_uuid();
    std::cout << "get_chat_server uid: " << request->uid() << ", gen token: " << token << std::endl;

    response->set_error(0);
    response->set_host(server.host);
    response->set_port(server.port);
    response->set_token(token);

    // 将token插入tokens
    tokens_[request->uid()] = token;

    std::cout << "get_chat_server finished" << std::endl;
    return grpc::Status::OK;
}

grpc::Status GrpcStatusServer::user_login(grpc::ServerContext *context, const UserLoginRequest *request, UserLoginResponse *response)
{
    std::lock_guard<std::mutex> locker(mutex_);
    response->set_error(MY_STATUS_CODE::SUCCESS);

    std::string uid = request->uid();
    std::string token = request->token();

    // 从tokens中查询uid对应的token是否存在
    if (tokens_.find(uid) == tokens_.end())
    {
        response->set_error(MY_STATUS_CODE::TOKEN_INVALID);
    }

    response->set_uid(uid);
    response->set_token(token);

    return grpc::Status::OK;
}
