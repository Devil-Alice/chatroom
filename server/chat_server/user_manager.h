#pragma once
#include "global.h"
#include "session.h"

class UserManager : public Singleton<UserManager>
{
    friend class Singleton<UserManager>;
    using string = std::string;
private:
    // key是用户uid，value时对应的session
    std::map<string, std::shared_ptr<Session>> uid_sessions_;
    UserManager();
    
public:

    // 传入用户uid以及对应的session
    void set_uid_session(string uid, std::shared_ptr<Session> session);
    std::shared_ptr<Session> get_uid_session(string uid);
    void remove_uid_session(string uid);
};