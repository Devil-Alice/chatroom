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
    // todo: 完成dto需要联查user取出用户姓名
    std::vector<std::shared_ptr<FriendApplyDto>> get_friend_applys_dto_by_uid(string uid);
    bool update_friend_apply_status(string from_uid, string to_uid, int status);
};