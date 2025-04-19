#pragma once
#include "global.h"

class FriendRelationDao : public Singleton<FriendRelationDao>
{
    friend class Singleton<FriendRelationDao>;
    using string = std::string;
public:
    bool add_friend(FriendRelation friend_relation);
    std::vector<std::shared_ptr<FriendRelation>> get_friends_by_uid(string uid);
};