#include "user_dao.h"
#include "mysql_pool.h"

User::User() {}

User::User(string uid, string name, string phone, string password)
{
    uid_ = uid;
    name_ = name;
    phone_ = phone;
    password_ = password;
}
User::~User() {}
int User::get_id() { return id_; }
std::string User::get_uid() { return uid_; }
void User::set_uid(string uid) { uid_ = uid; }
std::string User::get_name() { return name_; }
void User::set_name(string name) { name_ = name; }
std::string User::get_phone() { return phone_; }
void User::set_phone(string phone) { phone_ = phone; }
std::string User::get_password() { return password_; }
void User::set_password(string password) { password_ = password; }

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
    }
    catch (std::exception &ex)
    {
        std::cout << "add_user failed:" << ex.what() << std::endl;
        conn->conn_->rollback();
    }
    return true;
}

bool UserDao::delete_user(User &user)
{
    return false;
}

bool UserDao::update_user(User &user)
{
    return false;
}

User UserDao::get_user_by_id(string id)
{
    User user;
    return user;
}
