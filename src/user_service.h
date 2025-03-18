#pragma once
#include "user_dao.h"

class UserService : public Singleton<UserService>
{
    friend class Singleton<UserService>;
    using string = std::string;
private:
    UserDao &user_dao_;
    UserService();
public:
    ~UserService();
    bool register_user(User &user);
    string get_verify_code(string phone);
};