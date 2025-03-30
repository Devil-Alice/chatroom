#include "user_service.h"
#include "grpc_verify_client.h"
#include "grpc_status_client.h"

UserService::UserService() : user_dao_(UserDao::instance()), redis_(RedisManager::instance())
{
}

UserService::~UserService()
{
}

bool UserService::register_user(User &user)
{
    // 插入前先检查是否存在该用户
    std::shared_ptr<User> user_ptr = user_dao_.get_user_by_phone(user.get_phone());
    // 用户已存在
    if (user_ptr != nullptr)
    {
        throw std::runtime_error("phone already exists");
    }

    // 生成uid
    user.set_uid(my_utils::generate_uuid());

    // 添加用户
    bool success = user_dao_.add_user(user);
    return success;
}

std::string UserService::get_verify_code_from_redis(string phone)
{
    string verify_code = "";
    redis_.get(verify_code_prefix + phone, verify_code);
    std::cout << verify_code_prefix + phone << ": " << verify_code << std::endl;
    return verify_code;
}

std::string UserService::generate_verify_code(string phone)
{
    // todo: 写一个清除redis验证码的函数，避免用户在不同阶段混用验证码

    // 先在redis中查找
    string verify_code = get_verify_code_from_redis(phone);
    if (!verify_code.empty())
    {
        std::cout << verify_code_prefix + phone << " exist: " << verify_code << std::endl;
        return verify_code;
    }

    // redis中不存在，则向rpc获取
    std::cout << "request from grpc service" << std::endl;
    // 从grpc获取验证码
    GetVerifyResponse rsp = GrpcVerifyClient::instance().get_rerify_code(phone);
    if (rsp.error() != MY_STATUS_CODE::SUCCESS)
    {
        // rpc获取失败了
        throw std::runtime_error("get_rerify_code grpc request failed");
    }
    verify_code = rsp.code();

    // 获取成功，将验证码存入redis
    redis_.set(verify_code_prefix + phone, verify_code);
    redis_.expire(verify_code_prefix + phone, 60);

    return verify_code;
}

CommonResult UserService::user_login(string phone, string password)
{
    CommonResult result;

    auto user = user_dao_.get_user_by_phone(phone);
    if (user == nullptr)
        return result.set(MY_STATUS_CODE::USER_NOT_FOUND, "user not found");

    if (user->get_password() != password)
        return result.set(MY_STATUS_CODE::PASSWORD_ERROR, "password error");

    // 分配、获取聊天服务器的信息
    GetChatServerResponse rsp = GrpcStatusClient::instance().get_chat_server(user->get_uid());
    if (rsp.error() != MY_STATUS_CODE::SUCCESS)
        return result.set(MY_STATUS_CODE::RPC_FAILED, "rpc fauled");

    Json::Value json_result;
    json_result["uid"] = user->get_uid();
    json_result["name"] = user->get_name();
    json_result["phone"] = user->get_phone();
    json_result["host"] = rsp.host();
    json_result["port"] = rsp.port();
    json_result["token"] = rsp.token();


    return result.set(MY_STATUS_CODE::SUCCESS, "login success", json_result);
}
