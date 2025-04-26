#include "model.h"

User::User() {}

User::User(string name, string phone, string password)
{
    name_ = name;
    phone_ = phone;
    password_ = password;
    avatar_ = "";
}
User::User(string uid, string name, string phone, string password, string avatar)
{
    uid_ = uid;
    name_ = name;
    phone_ = phone;
    password_ = password;
    avatar_ = avatar;
}
User::~User() {}
int User::get_id() { return id_; }
std::string User::get_uid() { return uid_; }
void User::set_uid(string uid) { uid_ = uid; }
std::string User::get_name() { return name_; }
void User::set_name(string name) { name_ = name; }
std::string User::get_phone() { return phone_; }
void User::set_phone(string phone) { phone_ = phone; }
std::string User::get_password() { return password_; }
void User::set_password(string password) { password_ = password; }
std::string User::get_avatar() { return avatar_; }
void User::set_avatar(string avatar) { avatar_ = avatar; }

UserDto::UserDto()
{
}

UserDto::UserDto(std::string uid, std::string name, std::string avatar)
{
    uid_ = uid;
    name_ = name;
    avatar_ = avatar;
}

UserDto::UserDto(User &user)
{
    this->uid_ = user.get_uid();
    this->name_ = user.get_name();
    this->avatar_ = "";
}

Json::Value UserDto::to_json()
{
    Json::Value root;
    root["uid"] = uid_;
    root["name"] = name_;
    root["avatar"] = avatar_;
    return root;
}

void UserDto::from_json_string(std::string json_string)
{
}

FriendApply::FriendApply(string from_uid, string to_uid, string remark_name, string apply_message, int status)
{
    from_uid_ = from_uid;
    to_uid_ = to_uid;
    remark_name_ = remark_name;
    apply_message_ = apply_message;
    status_ = status;
}

FriendApplyDto::FriendApplyDto(UserDto from_user, UserDto to_user, string remark_name, string apply_message, int status)
{
    from_user_ = from_user;
    to_user_ = to_user;
    remark_name_ = remark_name;
    apply_message_ = apply_message;
    status_ = status;
}

Json::Value FriendApplyDto::to_json()
{
    Json::Value root;
    root["from_user"] = from_user_.to_json();
    root["to_user"] = to_user_.to_json();
    root["remark_name"] = remark_name_;
    root["apply_message"] = apply_message_;
    root["status"] = status_;
    return root;
}

void FriendApplyDto::from_json_string(std::string json_string)
{
}

FriendRelation::FriendRelation(string uid, string friend_uid, string remark_name)
{
    uid_ = uid;
    friend_uid_ = friend_uid;
    remark_name_ = remark_name;
}