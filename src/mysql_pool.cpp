#include "mysql_pool.h"

SqlConnection::SqlConnection(sql::Connection *conn, long int last_execute_time):
conn_(conn), last_execute_time_(last_execute_time)
{
}

MysqlPool::MysqlPool(size_t pool_size, string host, string port, string user, string password, string schema) : pool_size_(pool_size), host_(host), port_(port), user_(user), password_(password), schema_(schema)
{
    try
    {
        for (size_t i = 0; i < pool_size_; i++)
        {
            // 获取连接和时间戳，构造SqlConnection，push进connections

            // 获取mysql驱动
            sql::mysql::MySQL_Driver *mysql_driver = sql::mysql::get_mysql_driver_instance();
            // 通过驱动获取连接
            sql::Connection *conn = mysql_driver->connect(string("tcp://") + host_ + ":" + port_, user_, password_);
            conn->setSchema(schema_);

            long int current_time = get_current_time_s();

            // 推入connections
            connections_.push(new SqlConnection(conn, current_time));
        }

        heartbeat_thread_ = std::thread([this]()
                                        {
            while (!stop_flag_)
            {
                check_heartbeat();
                std::this_thread::sleep_for(std::chrono::seconds(60));
            } });

        heartbeat_thread_.detach();
    }
    catch (std::exception &ex)
    {
        std::cout << "MysqlPool exception: " << ex.what() << std::endl;
    }
}

MysqlPool::~MysqlPool()
{
    stop();
}

MysqlPool &MysqlPool::instance()
{
    static string host = app_config["mysql_server"]["host"];
    static string port = app_config["mysql_server"]["port"];
    static string user = app_config["mysql_server"]["user"];
    static string password = app_config["mysql_server"]["password"];
    static string schema = app_config["mysql_server"]["schema"];
    static MysqlPool instance_ = MysqlPool(10, host, port, user, password, schema);
    return instance_;
}

std::shared_ptr<SqlConnection> MysqlPool::get_connection()
{
    std::unique_lock<std::mutex> locker(mutex_);
    cond_.wait(locker, [this]() -> bool
               { return stop_flag_ || !connections_.empty(); });

    if (stop_flag_)
        return nullptr;

    SqlConnection *conn = connections_.front();
    connections_.pop();

    std::shared_ptr<SqlConnection> conn_ptr(conn, [this](SqlConnection *sql_conn)
                                            {
        std::lock_guard<std::mutex> locker(mutex_);
        connections_.push(sql_conn);
        cond_.notify_one(); });

    return conn_ptr;
}

void MysqlPool::stop()
{
    std::lock_guard<std::mutex> locker(mutex_);
    stop_flag_ = true;
    cond_.notify_all();

    while (!connections_.empty())
    {
        SqlConnection *conn = connections_.front();
        delete conn;
        connections_.pop();
    }
}

long int MysqlPool::get_current_time_s()
{
    // 获取自从time_since_epoch（1970-01-01 00:00:00 UTC）经过的时间
    auto time_point = std::chrono::system_clock::now().time_since_epoch();
    // 转化为秒数
    long int time_count = std::chrono::duration_cast<std::chrono::seconds>(time_point).count();
    return time_count;
}

void MysqlPool::check_heartbeat()
{
    std::lock_guard<std::mutex> locker(mutex_);
    long int current_time = get_current_time_s();

    // 将池子中现有的conn发送一次即可，被取出的本身就不需要发送
    int cnt = connections_.size();
    for (int i = 0; i < cnt; i++)
    {
        SqlConnection * conn = connections_.front();
        connections_.pop();
        Raii raii([this, &conn]()
                  { connections_.push(conn); });

        if ((current_time - conn->last_execute_time_) < 30)
        {
            continue;
        }

        try
        {
            // 如果这个连接的未操作时间大于等于30秒，则发送心跳包
            std::unique_ptr<sql::Statement> stmt(conn->conn_->createStatement());
            stmt->executeQuery("select 1;");
            conn->last_execute_time_ = current_time;
            std::cout << "mysql heartbeat sent" << std::endl;
        }
        catch (std::exception &ex)
        {
            // 发送失败，则使用新连接
            std::cout << "mysql heartbead send failed" << std::endl;
            // 获取mysql驱动
            sql::mysql::MySQL_Driver *mysql_driver = sql::mysql::get_mysql_driver_instance();
            // 通过驱动获取连接
            sql::Connection *new_conn = mysql_driver->connect(string("tcp://") + host_ + ":" + port_, user_, password_);
            new_conn->setSchema(schema_);
            conn->conn_.reset(new_conn);
            conn->last_execute_time_ = current_time;
        }
    }
}
