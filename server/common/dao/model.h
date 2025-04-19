#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include "json_object.h"

// todo: 添加头像

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
};

class SearchedUserDto : public JsonObject
{
public:
    std::string uid_;
    std::string name_;
    std::string avatar_;

    SearchedUserDto(User &user);
    Json::Value to_json() override;
    void from_json_string(std::string json_string) override;
};

class FriendApply
{
    using string = std::string;
public:
    string id_;
    string from_uid_;
    string to_uid_;
    string remark_name_;
    string apply_message_;
    //状态字段，0表示未处理， 1表示同意， 2表示拒绝
    int status_;

    FriendApply(string from_uid, string to_uid, string remark_name, string apply_message, int status = 0);
};