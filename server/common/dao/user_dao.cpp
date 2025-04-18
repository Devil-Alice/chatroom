#include "user_dao.h"
#include "mysql_pool.h"


UserDao::UserDao()
{
}

UserDao::~UserDao()
{
}

bool UserDao::add_user(User &user)
{
    std::shared_ptr<SqlConnection> conn = MysqlPool::instance().get_connection();
    if (conn == nullptr)
        return false;

    // raii自动关闭事务
    Raii raii([conn]()
              { conn->conn_->setAutoCommit(true); });

    // 开启事务(关闭自动提交)
    conn->conn_->setAutoCommit(false);

    try
    {

        // 获取预处理语句
        std::unique_ptr<sql::PreparedStatement> stmt(conn->conn_->prepareStatement("insert into user(uid, name, phone, password) values(?, ?, ?, ?);"));

        // 填入参数
        stmt->setString(1, user.get_uid());
        stmt->setString(2, user.get_name());
        stmt->setString(3, user.get_phone());
        stmt->setString(4, user.get_password());

        // 执行，获取影响的行数
        int affected_rows = stmt->executeUpdate();
        if (affected_rows != 1)
            return false;

        // 提交事务
        conn->conn_->commit();
        return true;
    }
    catch (std::exception &ex)
    {
        std::cout << "add_user failed:" << ex.what() << std::endl;
        conn->conn_->rollback();
    }
    return false;
}

bool UserDao::delete_user(User &user)
{
    return false;
}

bool UserDao::update_user(User &user)
{
    return false;
}

std::shared_ptr<User> UserDao::get_user_by_phone(string phone)
{
    auto conn = MysqlPool::instance().get_connection();
    if (conn == nullptr)
        throw std::runtime_error("get mysql connection failed");

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->conn_->prepareStatement("select * from user where phone = ?;"));
        pstmt->setString(1, phone);
        std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());

        if (!result->next())
        {
            return nullptr;
        }

        std::shared_ptr<User> user_ptr = std::make_shared<User>();
        user_ptr->set_uid(result->getString("uid"));
        user_ptr->set_name(result->getString("name"));
        user_ptr->set_phone(result->getString("phone"));
        user_ptr->set_password(result->getString("password"));

        return user_ptr;
    }
    catch (std::exception &ex)
    {
        std::cout << "add_user failed:" << ex.what() << std::endl;
    }

    return nullptr;
}

std::vector<std::shared_ptr<User>> UserDao::get_users_by_name(string name)
{
    auto conn = MysqlPool::instance().get_connection();
    std::vector<std::shared_ptr<User>> users;

    std::unique_ptr<sql::PreparedStatement> pstmt(conn->conn_->prepareStatement("select * from user where name like ?"));
    pstmt->setString(1, "%" + name + "%");
    std::unique_ptr<sql::ResultSet> result(pstmt->executeQuery());

    if (result->rowsCount() <= 0)
        return users;

    while (result->next())
    {
        string uid = result->getString("uid");
        string name = result->getString("name");
        string phone = result->getString("phone");
        string password = result->getString("password");
        users.emplace_back(new User(uid, name, phone, password));
    }

    return users;
}
