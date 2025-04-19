#include "friend_apply_dao.h"
#include "mysql_pool.h"

bool FriendApplyDao::update_friend_apply(FriendApply friend_apply)
{
    auto conn = MysqlPool::instance().get_connection();
    try
    {
        if (conn == nullptr)
        {
            return false;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->conn_->prepareStatement(
        " insert into friend_apply (from_uid, to_uid, remark_name, apply_message, status) "
        " values (?, ?, ?, ?, ?) "
        " on duplicate key "
        " update remark_name = values(remark_name), apply_message = values(apply_message), status = values(status); "));
        
        pstmt->setString(1, friend_apply.from_uid_);
        pstmt->setString(2, friend_apply.to_uid_);
        pstmt->setString(3, friend_apply.remark_name_);
        pstmt->setString(4, friend_apply.apply_message_);
        pstmt->setInt(5, friend_apply.status_);


        // 这里如果插入重复，则执行更新后的rowcount为2，如果单纯插入则为1
        int effect_rows = pstmt->executeUpdate();
        if (effect_rows >= 1)
            return true;
        
        return false;
    }
    catch (std::runtime_error &ex)
    {
        std::cout << "FriendApplyDao::update_friend_apply error: " << ex.what() << std::endl;
        return false;
    }

    return false;
}

std::vector<std::shared_ptr<FriendApply>> FriendApplyDao::get_friend_applys_by_uid(string uid)
{
    std::vector<std::shared_ptr<FriendApply>> friend_applys;
    auto conn = MysqlPool::instance().get_connection();

    try
    {
        if (conn == nullptr)
            return friend_applys;

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->conn_->prepareStatement("select * from apply_friend where from_uid = ?;"));
        pstmt->setString(1, uid);
        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());

        while (result->next())
        {
            string from_uid = result->getString("from_uid");
            string to_uid = result->getString("to_uid");
            string remark_name = result->getString("remark_name");
            string apply_message = result->getString("apply_message");
            int status = result->getInt("status");
            friend_applys.emplace_back(new FriendApply(from_uid, to_uid, remark_name, apply_message, status));
        }


        return friend_applys;
    }
    catch (std::runtime_error &ex)
    {
        std::cout << "FriendApplyDao::update_friend_apply error: " << ex.what() << std::endl;
        return friend_applys;
    }

    return friend_applys;
}
