#include "tcp_service.h"
#include "user_service.h"
#include "grpc_status_client.h"

TcpService::TcpService()
{
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
}

TcpService::~TcpService()
{
}

void TcpService::register_service(REQUEST_ID request_id, TcpHandler handler)
{
    services_.insert(std::make_pair(request_id, handler));
    return;
}

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
