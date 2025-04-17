#include "user.h"


User::User() {}

User::User(string name, string phone, string password)
{
    name_ = name;
    phone_ = phone;
    password_ = password;
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

SearchedUserDto::SearchedUserDto(User &user)
{
    this->name_ = user.get_name();
    this->avatar_ = "";
}

Json::Value SearchedUserDto::to_json()
{
    Json::Value root;
    root["name"] = name_;
    root["avatar"] = avatar_;
    return root;
}

void SearchedUserDto::from_json_string(std::string json_string)
{
}
