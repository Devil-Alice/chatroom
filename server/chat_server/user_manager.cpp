#include "user_manager.h"

UserManager::UserManager()
{
}

void UserManager::set_uid_session(string uid, std::shared_ptr<Session> session)
{
    if (session != nullptr)
        uid_sessions_[uid] = session;
}

std::shared_ptr<Session> UserManager::get_uid_session(string uid)
{
    // 避免[]查询，因为[]当key不存在时，会插入一个新的key
    if (uid_sessions_.find(uid) == uid_sessions_.end())
        return nullptr;
    
    return uid_sessions_[uid];
}

void UserManager::remove_uid_session(string uid)
{
    uid_sessions_.erase(uid);
}
