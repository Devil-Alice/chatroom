#include "user_service.h"
#include "grpc_verify_client.h"
#include "grpc_status_client.h"

UserService::UserService() : user_dao_(UserDao::instance()), friend_apply_dao_(FriendApplyDao::instance()), redis_(RedisManager::instance())
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

CommonResult UserService::chat_login()
{
    return CommonResult();
}

CommonResult UserService::search_content(string content)
{
    CommonResult result;
    try
    {
        std::vector<std::shared_ptr<User>> users;

        bool is_digit = my_utils::is_all_digit(content);

        // 如果全是数字，根手机号查询
        if (is_digit)
        {
            auto user = user_dao_.get_user_by_phone(content);
            if (user != nullptr)
                users.push_back(user);
        }

        // 不管是不是数字，都要根据name查询
        auto users_by_name = user_dao_.get_users_by_name(content);
        users.insert(users.end(), users_by_name.begin(), users_by_name.end());

        // 将user转换为JsonObject
        std::vector<std::shared_ptr<JsonObject>> searched_users;
        for (std::shared_ptr<User> item : users)
        {
            searched_users.emplace_back(new UserDto(*item));
        }

        // 将vector转为json
        Json::Value json_data = JsonObject::to_json_array(searched_users);

        return result.set(MY_STATUS_CODE::SUCCESS, "query success", json_data);
    }
    catch (std::runtime_error &ex)
    {
        return result.set(MY_STATUS_CODE::ERROR, ex.what());
    }

    return result;
}

CommonResult UserService::update_friend_apply(string from_uid, string to_uid, string remark_name, string apply_message, int status)
{
    CommonResult result(MY_STATUS_CODE::ERROR, "");

    FriendApply apply(from_uid, to_uid, remark_name, apply_message, status);
    bool success = friend_apply_dao_.update_friend_apply(apply);

    if (!success)
        return result.set(MY_STATUS_CODE::DATABASE_FAILED, "failed");

    return result.set(MY_STATUS_CODE::SUCCESS, "ok");
}

CommonResult UserService::get_friend_applys_by_uid(string uid)
{
    CommonResult result(MY_STATUS_CODE::ERROR, "");

    std::vector<std::shared_ptr<FriendApplyDto>> applys = friend_apply_dao_.get_friend_applys_dto_by_uid(uid);

    std::vector<std::shared_ptr<JsonObject>> json_array;
    for (auto item : applys)
    {
        json_array.push_back(item);
    }  
    
    return result.set(MY_STATUS_CODE::SUCCESS, "query success", JsonObject::to_json_array(json_array));
}
