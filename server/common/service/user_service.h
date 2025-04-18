#pragma once
#include "user_dao.h"
#include "redis_manager.h"

// todo: 给这个类中的函数加上异常处理

class UserService : public Singleton<UserService>
{
    friend class Singleton<UserService>;
    using string = std::string;
private:
    UserDao &user_dao_;
    RedisManager &redis_;
    UserService();
public:
    ~UserService();
    bool register_user(User &user);
    string get_verify_code_from_redis(string phone);
    string generate_verify_code(string phone);
    // 用户登陆，验证用户的手机号、密码
    CommonResult user_login(string phone, string password);
    // todo:chat_login待完成
    CommonResult chat_login();
    CommonResult search_content(string content);
};