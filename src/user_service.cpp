#include "user_service.h"
#include "redis_manager.h"
#include "verify_grpc_client.h"

UserService::UserService() : user_dao_(UserDao::instance())
{
}

UserService::~UserService()
{
}

bool UserService::register_user(User &user)
{
    // 插入前先检查是否存在该用户
    std::shared_ptr<User> user_ptr = user_dao_.get_user_by_name(user.get_name());
    // 用户已存在
    if (user_ptr != nullptr)
    {
        throw std::runtime_error("user is already exist");
    }

    // 添加用户
    bool success = user_dao_.add_user(user);
    return success;
}

std::string UserService::get_verify_code(string phone)
{
    // 现在redis中查找
    RedisManager &redis = RedisManager::instance();
    string verify_code = "";
    bool success = redis.get(verify_code_prefix + phone, verify_code);
    // redis中存在，直接返回
    if (success && !verify_code.empty())
    {
        return verify_code;
    }

    // redi中不存在，则向rpc获取
    std::cout << verify_code_prefix + phone << " dosen't exist" << std::endl;
    std::cout << "request from grpc service" << std::endl;
    // 从grpc获取验证码
    GetVerifyResponse rsp = VerifyGrpcClient::instance().get_rerify_code(phone);
    if (rsp.error() != MY_STATUS_CODE::SUCCESS)
    {
        // rpc获取失败了
        throw std::runtime_error("get_rerify_code grpc request failed");
    }
    verify_code = rsp.code();

    // 获取成功，将验证码存入redis
    redis.set(verify_code_prefix + phone, verify_code);

    return verify_code;
}
