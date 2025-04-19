#pragma once
#include "global.h"

class FriendApplyDao : public Singleton<FriendApplyDao>
{
    friend class Singleton<FriendApplyDao>;
    using string = std::string;
public:
    // 插入好友申请，如果存在，则更新
    bool update_friend_apply(FriendApply friend_apply);
    std::vector<std::shared_ptr<FriendApply>> get_friend_applys_by_uid(string uid);
};