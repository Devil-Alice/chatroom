#include "tcp_service.h"
#include "user_service.h"
#include "grpc_status_client.h"
#include "user_manager.h"
#include "grpc_chat_client.h"

TcpService::TcpService()
{
    // 注册模版，作为其他注册动作的示例
    register_service(REQUEST_ID::UNKNOWN,
        [](std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
        {
            CommonResult result;

            auto json_request = JsonObject::parse_json_string(request_package->get_message());
            // 每条请求都会附加验证的uid和token
            string uid = json_request["uid"].asString();
            string token = json_request["token"].asString();
    

            return result;
        });

    register_service(REQUEST_ID::CHAT_LOGIN, 
        [](std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
        {
        CommonResult result;
        // 获取请求信息
        string msg = request_package->get_message();
        Json::Value json_request = JsonObject::parse_json_string(msg);
        
        // 查询数据库，用户是否存在
        string uid = json_request["uid"].asString();
        string token = json_request["token"].asString();
        
        result = UserService::instance().chat_login(uid, token, session->get_server_name());
        
        // 登陆不成功，返回
        if (result.get_status() != MY_STATUS_CODE::SUCCESS)
            return result;

        // 如果登陆成功，则设置session的user uid，以及向status服务发送请求
        session->set_user_uid(uid);

        // 登陆成功后，还需要向usermanager中注册映射
        UserManager::instance().set_uid_session(uid, session);

        return result;
    });


    register_service(REQUEST_ID::SEARCH_CONTENT, 
    [](std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
    {
        // 根据package中的内容作处理

        Json::Value json_request = JsonObject::parse_json_string(request_package->get_message());
        string search_content = json_request["search_content"].asString();

        return UserService::instance().search_content(search_content);
    });



    register_service(REQUEST_ID::SNED_FRIEND_APPLY,
    [](std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
    {
        auto json_request = JsonObject::parse_json_string(request_package->get_message());
        
        string from_uid = json_request["from_uid"].asString();
        string uid = from_uid;
        string token = json_request["token"].asString();
        string to_uid = json_request["to_uid"].asString();
        string remark_name = json_request["remark_name"].asString();
        string apply_message = json_request["apply_message"].asString();

        auto result = UserService::instance().update_friend_apply(from_uid, to_uid, remark_name, apply_message);


        // 好友申请完毕，向对面发送即时通知--------------------------------------------------------

        // 获取请求人的信息
        auto from_user_ptr = UserDao::instance().get_user_by_uid(from_uid);
        FriendApplyDto apply_dto;
        apply_dto.from_user_.uid_ = from_user_ptr->get_uid();
        apply_dto.from_user_.name_ = from_user_ptr->get_name();
        apply_dto.from_user_.avatar_ = from_user_ptr->get_avatar();
        apply_dto.to_user_.uid_ = to_uid;
        apply_dto.apply_message_ = apply_message;
        Json::Value notify_json = apply_dto.to_json();
        CommonResult notify_result(MY_STATUS_CODE::SUCCESS, "", notify_json);

        // 获取对方的session
        std::shared_ptr<Session> to_session = UserManager::instance().get_uid_session(to_uid);
        std::cout << "send notify..." << std::endl;
        
        if (to_session)
        {
            std::cout << "user is in this server" << std::endl;
            // 构造提醒数据包
            std::shared_ptr<Package> notify_package(new Package());
            notify_package->set_request_id(REQUEST_ID::NOTIFY_SEND_FRIEND_APPLY);
            notify_package->set_message(notify_result.to_json_string());
            to_session->add_response(notify_package);
        }
        else
        {
            // 没找到session，说明对方不在同一服务器，需要通过grpc服务器通知对方的服务器发送这个数据包给该用户
            // 先通过redis查询这个用户在哪台服务器上，如果查不到，说明不在线，不需要再发送了
            string server_name = RedisManager::instance().hget(user_login_server_hset_key, to_uid);

            if (server_name.empty())
                return result;

            std::cout << "user is in another server: " << server_name << std::endl;
            
            NotifyRequest req;
            req.set_from_uid(from_uid);
            req.set_token(token);
            req.set_to_uid(to_uid);
            req.set_request_id(REQUEST_ID::NOTIFY_SEND_FRIEND_APPLY);
            req.set_message(notify_result.to_json_string());
            auto rsp = GrpcChatClient::instance().notify(server_name, req);
            if (rsp.error() == MY_STATUS_CODE::SUCCESS)
            {
                std::cout << "rpc notify failed" << std::endl;
                return result;
            }
        }

        return result;
    });


    register_service(REQUEST_ID::QUERY_FRIEND_APPLY,
        [](std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
        {

            Json::Value json_request = JsonObject::parse_json_string(request_package->get_message());
            string uid = json_request["uid"].asString();

            return UserService::instance().get_friend_applys_by_uid(uid);
        });


    register_service(REQUEST_ID::HANDLE_FRIEND_APPLY,
        [](std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
        {        
            auto json_request = JsonObject::parse_json_string(request_package->get_message());
            string uid = json_request["uid"].asString();
            string token = json_request["token"].asString();
            string from_uid = json_request["from_uid"].asString();
            string to_uid = json_request["to_uid"].asString();
            string remark_name = json_request["remark_name"].asString();
            int status = json_request["status"].asInt();

            return UserService::instance().handle_friend_apply(from_uid, to_uid, status, remark_name);
        });

}

TcpService::~TcpService()
{
}

void TcpService::register_service(REQUEST_ID request_id, TcpHandler handler)
{
    services_.insert(std::make_pair(request_id, handler));
    return;
}

// 该函数需要在外层添加异常处理
std::shared_ptr<Package> TcpService::handle_request(std::shared_ptr<Session> session, std::shared_ptr<Package> request_package)
{
    REQUEST_ID request_id = (REQUEST_ID)request_package->get_request_id();

    // 设置默认返回数据
    CommonResult result(MY_STATUS_CODE::ERROR, "request not found");
    std::shared_ptr<Package> response_package(new Package());
    response_package->set_request_id(request_id);
    response_package->set_message(result.to_json_string());


    if (services_.find(request_id) == services_.end())
    {
        std::cout << "tcp reqest(" << request_id << ") not found" << std::endl;
        return response_package;
    }

    result = services_[request_id](session, request_package);
    response_package->set_message(result.to_json_string());
    return response_package;
}