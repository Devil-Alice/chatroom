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
        // std::cout << info.host << " " << info.port << std::endl;
        chat_servers_infos_.push_back(info);
    }

}

GrpcStatusServer::~GrpcStatusServer()
{
}

grpc::Status GrpcStatusServer::get_chat_server(grpc::ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response)
{
    // todo: server是公共资源，而grpc处理多个任务是多线程，所以需要加锁
    std::lock_guard<std::mutex> locker(mutex_);
    std::cout << "get_chat_server uid:" << request->uid() << std::endl;
    ChatServerInfo server = chat_servers_infos_[poll_idx_];
    poll_idx_ = (poll_idx_ + 1) % chat_servers_infos_.size();

    // 用户请求聊天服务器的时候，给他生成一个token，传回
    string token = my_utils::generate_uuid();

    response->set_error(0);
    response->set_host(server.host);
    response->set_port(std::to_string(server.port));
    response->set_token(token);

    // 将token插入tokens
    tokens_[request->uid()] = token;

    std::cout << "get_chat_server finished" << std::endl;
    return grpc::Status::OK;
}

grpc::Status GrpcStatusServer::user_login(grpc::ServerContext *context, const UserLoginRequest *request, UserLoginResponse *response)
{

    std::string uid = request->uid();
    std::string token = request->token();

    return grpc::Status::OK;
}
