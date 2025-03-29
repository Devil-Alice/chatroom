#pragma once
#include "global.h"

class User
{
    using string = std::string;

private:
    int id_;
    string uid_;
    string name_;
    string phone_;
    string password_;

public:
    User();
    User(string name, string phone, string password);
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

class UserDao : public Singleton<UserDao>
{
    friend class Singleton<UserDao>;
    using string = std::string;

private:
    UserDao();
public:
    ~UserDao();
    bool add_user(User &user);
    bool delete_user(User &user);
    bool update_user(User &user);
    std::shared_ptr<User> get_user_by_phone(string phone);
};