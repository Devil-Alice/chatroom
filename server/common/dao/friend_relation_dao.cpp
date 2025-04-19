#include "friend_relation_dao.h"
#include "mysql_pool.h"

bool FriendRelationDao::add_friend(FriendRelation friend_relation)
{
    auto conn = MysqlPool::instance().get_connection();

    try
    {
        if (conn == nullptr)
            return false;

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->conn_->prepareStatement(
            "insert into friend_relation (uid, friend_uid, remark_name) values (?, ?, ?);"));

        pstmt->setString(1, friend_relation.uid_);
        pstmt->setString(2, friend_relation.friend_uid_);
        pstmt->setString(3, friend_relation.remark_name_);
        int effect_rows = pstmt->executeUpdate();

        if (effect_rows == 1)
            return true;
    }
    catch (std::runtime_error &ex)
    {
        std::cout << "FriendRelationDao::add_friend error: " << ex.what() << std::endl;
        return false;
    }

    return false;
}

std::vector<std::shared_ptr<FriendRelation>> FriendRelationDao::get_friends_by_uid(string uid)
{
    auto conn = MysqlPool::instance().get_connection();
    std::vector<std::shared_ptr<FriendRelation>> friends;
    try
    {
        if (conn == nullptr)
            return friends;

        std::shared_ptr<sql::PreparedStatement> pstmt(conn->conn_->prepareStatement(
            "select * from friend_relation where uid = ?;"));
        pstmt->setString(1, uid);

        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());

        while (result->next())
        {
            // 此处加上前缀，与参数区分
            string _uid = result->getString("uid");
            string friend_uid = result->getString("friend_uid");
            string remark_name = result->getString("remark_name");
            friends.emplace_back(new FriendRelation(uid, friend_uid, remark_name));
        }

        return friends;
    }
    catch (std::runtime_error &ex)
    {
        std::cout << "FriendRelationDao::get_friends_by_uid error: " << ex.what() << std::endl;
        return friends;
    }

    return friends;
}
