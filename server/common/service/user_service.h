#pragma once
#include "user_dao.h"
#include "friend_apply_dao.h"
#include "friend_relation_dao.h"
#include "redis_manager.h"

// todo: 给这个类中的函数加上异常处理

class UserService : public Singleton<UserService>
{
    friend class Singleton<UserService>;
    using string = std::string;

private:
    UserDao &user_dao_;
    FriendApplyDao &friend_apply_dao_;
    FriendRelationDao friend_relation_dao_;
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
    // 插入好友申请，如果存在，则更新
    CommonResult update_friend_apply(string from_uid, string to_uid, string remark_name, string apply_message, int status = 0);
    CommonResult get_friend_applys_by_uid(string uid);
    // 处理好友申请，接收或拒绝
    CommonResult handle_friend_apply(string from_uid, string to_uid, int status, string remark_name = "");
    CommonResult add_friend_relation(string from_uid, string to_uid, string remark_name);
};