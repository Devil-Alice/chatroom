#pragma once
#include "global.h"

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
    std::vector<std::shared_ptr<User>> get_users_by_name(string name);
};