#pragma once
#include "global.h"

class SqlConnection
{
public:
    std::unique_ptr<sql::Connection> conn_;
    long int last_execute_time_;

    SqlConnection(sql::Connection *conn, long int last_execute_time);
};

class MysqlPool
{
    using string = std::string;

private:
    size_t pool_size_;
    string host_;
    string port_;
    string user_;
    string password_;
    // schema_就是数据库名
    string schema_;
    std::queue<SqlConnection *> connections_;

    std::thread heartbeat_thread_;
    std::atomic<bool> stop_flag_;
    std::mutex mutex_;
    std::condition_variable cond_;

    MysqlPool(size_t pool_size, string host, string port, string user, string password, string db_name);
public:
    ~MysqlPool();
    static MysqlPool& instance();
    std::shared_ptr<SqlConnection> get_connection();

private:
    void stop();
    long int get_current_time_s();
    /// @brief 检查是否发送心跳包
    /// @note heartbeat_thread_每60秒检测一次，如果连接超过30秒没有操作，则发送心跳包
    void check_heartbeat();

};