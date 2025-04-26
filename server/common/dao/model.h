#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include "json_object.h"

// todo: 添加头像的存储逻辑
// todo: 实现dto的反序列化，让客户端和服务器传输统一使用序列化的对象，提高复用性

class User
{
    using string = std::string;

private:
    int id_;
    string uid_;
    string name_;
    string phone_;
    string password_;
    string avatar_;

public:
    User();
    User(string name, string phone, string password);
    User(string uid, string name, string phone, string password, string avatar = "");
    ~User();

    int get_id();
    string get_uid();
    void set_uid(string uid);
    string get_name();
    void set_name(string name);
    string get_phone();
    void set_phone(string phone);
    string get_password();
    void set_password(string password);
    string get_avatar();
    void set_avatar(string avatar);
};

class UserDto : public JsonObject
{
public:
    std::string uid_;
    std::string name_;
    std::string avatar_;

    UserDto();
    UserDto(std::string uid, std::string name, std::string avatar);
    UserDto(User &user);
    Json::Value to_json() override;
    void from_json_string(std::string json_string) override;
};

class FriendApply
{
    using string = std::string;

public:
    int id_;
    string from_uid_;
    string to_uid_;
    string remark_name_;
    string apply_message_;
    // 状态字段，0表示未处理， 1表示同意， 2表示拒绝
    int status_;

    FriendApply(string from_uid, string to_uid, string remark_name, string apply_message, int status = 0);
};

class FriendApplyDto : public JsonObject
{
    using string = std::string;

public:
    UserDto from_user_;
    UserDto to_user_;
    string remark_name_;
    string apply_message_;
    // 状态字段，0表示未处理， 1表示同意， 2表示拒绝
    int status_;

    FriendApplyDto(UserDto from_user, UserDto to_user, string remark_name, string apply_message, int status = 0);
    Json::Value to_json() override;
    void from_json_string(std::string json_string) override;
};

class FriendRelation
{
    using string = std::string;

public:
    int id_;
    string uid_;
    string friend_uid_;
    string remark_name_;

    FriendRelation(string uid, string friend_uid, string remark_name);
};
