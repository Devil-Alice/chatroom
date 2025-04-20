#include "tcp_service.h"
#include "user_service.h"
#include "grpc_status_client.h"

TcpService::TcpService()
{


    // 注册模版，作为其他注册动作的示例
    register_service(REQUEST_ID::UNKNOWN,
        [](std::shared_ptr<Package> package)
        {
            std::shared_ptr<Package> result_pkg(new Package());
            CommonResult result;

            auto json_request = JsonObject::parse_json_string(package->get_message());
            // 每条请求都会附加验证的uid和token
            string uid = json_request["uid"].asString();
            string token = json_request["token"].asString();
    
    
            result_pkg->set_request_id(package->get_request_id());
            result_pkg->set_message(result.to_json_string());
            return result_pkg;
        });

    register_service(REQUEST_ID::CHAT_LOGIN, [](std::shared_ptr<Package> package){
        //构建结果package的message
        CommonResult result;
        
        // 获取请求信息
        string msg = package->get_message();
        Json::Value json_request = JsonObject::parse_json_string(msg);
        
        // 查询数据库，用户是否存在
        string phone = json_request["phone"].asString();
        string password = json_request["password"].asString();
        string token = json_request["token"].asString();
        
        auto user = UserDao::instance().get_user_by_phone(phone);
        // 没查到用户，设置错误
        if (user == nullptr)
        {
            result.set(MY_STATUS_CODE::USER_NOT_FOUND, "user not found");
        }
        else
        {
            // 查到了用户，就根据uid查token
            // 从状态服务获取该用户的token
            // 匹配token是否正确
            UserLoginResponse rsp = GrpcStatusClient::instance().user_login(user->get_uid(), token);
            
            // 将rsp的错误设置到result中
            if (rsp.error() != MY_STATUS_CODE::SUCCESS)
            {
                result.set(MY_STATUS_CODE::TOKEN_INVALID, "token invalid");
            }
            else 
            {
                // token查询成功，将uid和token返回给用户
                Json::Value json_result;
                json_result["uid"] = user->get_uid();
                json_result["token"] = token;

                result.set(MY_STATUS_CODE::SUCCESS, "login success", json_result);
            }
        }

        // 先构建一个package
        std::shared_ptr<Package> result_pkg = std::make_shared<Package>();
        result_pkg->set_request_id(package->get_request_id());
        result_pkg->set_message(result.to_json_string());

        return result_pkg;
    });


    register_service(REQUEST_ID::SEARCH_CONTENT, 
    [](std::shared_ptr<Package> package){
        std::shared_ptr<Package> result_pkg(new Package());
        CommonResult result;
        
        // 根据package中的内容作处理

        Json::Value json_request = JsonObject::parse_json_string(package->get_message());
        string search_content = json_request["search_content"].asString();

        result = UserService::instance().search_content(search_content);

        result_pkg->set_request_id(package->get_request_id());
        result_pkg->set_message(result.to_json_string());

        return result_pkg;
    });



    register_service(REQUEST_ID::SNED_FRIEND_APPLY,
    [](std::shared_ptr<Package> package)
    {
        std::shared_ptr<Package> result_pkg(new Package());
        CommonResult result;

        auto json_request = JsonObject::parse_json_string(package->get_message());
        
        string from_uid = json_request["from_uid"].asString();
        string uid = from_uid;
        string token = json_request["token"].asString();
        string to_uid = json_request["to_uid"].asString();
        string remark_name = json_request["remark_name"].asString();
        string apply_message = json_request["apply_message"].asString();

        result = UserService::instance().update_friend_apply(from_uid, to_uid, remark_name, apply_message);

        result_pkg->set_request_id(package->get_request_id());
        result_pkg->set_message(result.to_json_string());
        return result_pkg;
    });


    register_service(REQUEST_ID::QUERY_FRIEND_APPLY,
        [](std::shared_ptr<Package> package)
        {
            std::shared_ptr<Package> result_pkg(new Package());
            CommonResult result;

            Json::Value json_request = JsonObject::parse_json_string(package->get_message());
            string uid = json_request["uid"].asString();

            result = UserService::instance().get_friend_applys_by_uid(uid);

            result_pkg->set_request_id(package->get_request_id());
            result_pkg->set_message(result.to_json_string());
            return result_pkg;
        });


        register_service(REQUEST_ID::HANDLE_FRIEND_APPLY,
            [](std::shared_ptr<Package> package)
            {
                std::shared_ptr<Package> result_pkg(new Package());
                CommonResult result;
        
                auto json_request = JsonObject::parse_json_string(package->get_message());
                string uid = json_request["uid"].asString();
                string token = json_request["token"].asString();

                string from_uid = json_request["from_uid"].asString();
                string to_uid = json_request["to_uid"].asString();
                int status = json_request["status"].asInt();

                result = UserService::instance().handle_friend_apply(from_uid, to_uid, status);
        
                result_pkg->set_request_id(package->get_request_id());
                result_pkg->set_message(result.to_json_string());
                return result_pkg;
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
std::shared_ptr<Package> TcpService::handle_request(std::shared_ptr<Package> package)
{
    REQUEST_ID request_id = (REQUEST_ID)package->get_request_id();
    if (services_.find(request_id) == services_.end())
    {
        std::cout << "tcp reqest(" << request_id << ") not found" << std::endl;
        return nullptr;
    }

    return services_[request_id](package);
}
