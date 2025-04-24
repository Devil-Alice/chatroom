#include "grpc_status_server.h"
#include "redis_manager.h"

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
        info.name = it->get("name", "").asString();
        info.host = it->get("host", "").asString();
        info.port = it->get("port", 0).asInt();
        info.grpc_chat_server_port = it->get("grpc_chat_server_port", 0).asInt();
        std::cout << info.host << " " << info.port << std::endl;
        chat_server_infos_.push_back(info);
    }
}

GrpcStatusServer::~GrpcStatusServer()
{
}

grpc::Status GrpcStatusServer::get_chat_server(grpc::ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response)
{
    // chat_servers_infos_是公共资源，而grpc处理多个任务是多线程，如果这个grpc服务器多个任务到来，可能会导致资源竞争，所以需要加锁
    std::lock_guard<std::mutex> locker(mutex_);

    // 通过查询redis获取人数最少的服务器信息
    ChatServerInfo server = chat_server_infos_[0];
    
    string server_name_min = "";
    int server_user_count_min = -1;
    for (int i = 0; i < chat_server_infos_.size(); i++)
    {
        string server_name = chat_server_infos_[i].name;
        string count_str = RedisManager::instance().hget(chat_server_user_count_hset_key, server_name);
        int count = std::atoi(count_str.data());

        // 如果是为初始化，或者是找到了新的最小值，记录下来
        if (server_user_count_min == -1 || count < server_user_count_min)
        {
            server_name_min = server_name;
            server_user_count_min = count;
            server = chat_server_infos_[i];
        }

    }

    // 用户请求聊天服务器的时候，给他生成一个token，传回
    string token = my_utils::generate_uuid();
    std::cout << "get_chat_server uid: " << request->uid() << ", gen token: " << token << std::endl;

    // 将token存入redis
    RedisManager::instance().hset(user_token_hset_key, request->uid(), token);

    // 返回数据
    response->set_error(0);
    response->set_host(server.host);
    response->set_port(server.port);
    response->set_token(token);

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
    string token_find = RedisManager::instance().hget(user_token_hset_key, uid);
    if (token_find != token)
    {
        response->set_error(MY_STATUS_CODE::TOKEN_INVALID);
    }

    response->set_uid(uid);
    response->set_token(token);

    return grpc::Status::OK;
}
