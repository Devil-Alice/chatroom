#include "tcp_service.h"
#include "user_service.h"
#include "grpc_status_client.h"

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
        session->set_user_uid((result.get_data())["uid"].asString());

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

        return UserService::instance().update_friend_apply(from_uid, to_uid, remark_name, apply_message);
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
    std::shared_ptr<Package> response_package;
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